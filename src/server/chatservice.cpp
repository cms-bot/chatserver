/*
* @Author: 快出来了哦
* @Date: 2023-09-20 14:27:31
 * @LastEditors: 快出来了哦
 * @LastEditTime: 2023-09-24 15:47:19
 * @FilePath: /chatserver/src/server/chatservice.cpp
* @Description: 
*/

#include "chatservice.hpp"
#include "globle.hpp"

#include <functional>
#include <muduo/base/Logging.h>
using namespace muduo;
using namespace std::placeholders;

ChatService::ChatService()
{
    // 用户基本业务管理相关事件处理回调注册
    _msghandleMap.insert({LOGIN_MSG,std::bind(&ChatService::login,this,_1,_2,_3)});
    _msghandleMap.insert({REG_MSG,std::bind(&ChatService::regist,this,_1,_2,_3)});
    _msghandleMap.insert({LOGINOUT_MSG, std::bind(&ChatService::loginout, this, _1, _2, _3)});
    _msghandleMap.insert({ONE_CHAT_MSG, std::bind(&ChatService::oneChat, this, _1, _2, _3)});
    _msghandleMap.insert({ADD_FRIEND_MSG, std::bind(&ChatService::addFriend, this, _1, _2, _3)});
    
    // 群组业务管理相关事件处理回调注册
    _msghandleMap.insert({CREATE_GROUP_MSG, std::bind(&ChatService::createGroup, this, _1, _2, _3)});
    _msghandleMap.insert({ADD_GROUP_MSG, std::bind(&ChatService::addGroup, this, _1, _2, _3)});
    _msghandleMap.insert({GROUP_CHAT_MSG, std::bind(&ChatService::groupChat, this, _1, _2, _3)});
}

//获取单例对象
ChatService* ChatService::instance()
{
    static ChatService service;
    return &service;
}

//获取消息对应的处理器
MsgHandler ChatService::getHandler(int msgid)
{
    auto it = _msghandleMap.find(msgid);
    if(it == _msghandleMap.end())
    {
        return [=](const TcpConnectionPtr &conn, json &js, Timestamp time)
        {
            LOG_ERROR << "msgid:" << msgid << " can not find handler!";
        };
    }
    return _msghandleMap[msgid];
}

//处理登录业务
void ChatService::login(const TcpConnectionPtr &conn, json &js, Timestamp time)
{
    int id = js["id"].get<int>();
    std::string pwd = js["password"];

    User user = _usermodel.query(id);
    if(user.getId() == id && user.getPwd() == pwd)
    {
        if(user.getState() == "online")//用户已经登录
        {
            json response;
            response["msgid"] = LOGIN_MSG_ACK;
            response["errno"] = 2;
            response["errmsg"] = "this account is using,input another!";
            conn->send(response.dump());
        }else
        {
            //登录成功，修改用户状态
            {
                std::unique_lock<std::mutex> lock(_connMutex);
                _userConnMap[id] = conn;
            }
            user.setState("online");
            _usermodel.updateState(user);

            json response;
            response["msgid"] = LOGIN_MSG_ACK;
            response["error"] = 0;
            response["id"] = user.getId();
            response["name"] = user.getName();

            //查询用户是否有离线消息
            std::vector<std::string> vec = _offlinMsgModel.query(id);
            if(!vec.empty())
            {
                response["offlinemsg"] = vec;
                _offlinMsgModel.remove(id);
            }
        
            // 查询该用户的好友信息并返回
            std::vector<User> userVec = _friendModel.query(id);
            if (!userVec.empty())
            {
                std::vector<string> vec2;
                for (User &user : userVec)
                {
                    json js;
                    js["id"] = user.getId();
                    js["name"] = user.getName();
                    js["state"] = user.getState();
                    vec2.push_back(js.dump());
                }
                response["friends"] = vec2;
            }

            //查询用户群组消息
            std::vector<Group> groupuserVec = _groupModel.queryGroups(id);
            if (!groupuserVec.empty())
            {
                // group:[{groupid:[xxx, xxx, xxx, xxx]}]
                vector<string> groupV;
                for (Group &group : groupuserVec)
                {
                    json grpjson;
                    grpjson["id"] = group.getId();
                    grpjson["groupname"] = group.getName();
                    grpjson["groupdesc"] = group.getDesc();
                    vector<string> userV;
                    for (GroupUser &user : group.getUsers())
                    {
                        json js;
                        js["id"] = user.getId();
                        js["name"] = user.getName();
                        js["state"] = user.getState();
                        js["role"] = user.getRole();
                        userV.push_back(js.dump());
                    }
                    grpjson["users"] = userV;
                    groupV.push_back(grpjson.dump());
                }

                response["groups"] = groupV;
            }

            conn->send(response.dump());
         }
    }
    else{
        // 该用户不存在，用户存在但是密码错误，登录失败
        json response;
        response["msgid"] = LOGIN_MSG_ACK;
        response["errno"] = 1;
        response["errmsg"] = "id or password is invalid!";
        conn->send(response.dump());
    }

}

//处理注册业务
void ChatService::regist(const TcpConnectionPtr &conn, json &js, Timestamp time)
{
    string name = js["name"];
    string pwd = js["password"];

    User user;
    user.setName(name);
    user.setPwd(pwd);
    bool state = _usermodel.insert(user);
    if (state)
    {
        // 注册成功
        json response;
        response["msgid"] = REG_MSG_ACK;
        response["errno"] = 0;
        response["id"] = user.getId();
        conn->send(response.dump());
    }
    else
    {
        // 注册失败
        json response;
        response["msgid"] = REG_MSG_ACK;
        response["errno"] = 1;
        conn->send(response.dump());
    }
}

//处理客户端异常退出
void ChatService::clientCloseException(const TcpConnectionPtr& conn)
{
    User user;
    {
        std::unique_lock<std::mutex> lock(_connMutex);
        for(auto it = _userConnMap.begin(); it != _userConnMap.end(); ++it)
        {
            if(it->second == conn)
            {
                //从map中删除
                user.setId(it->first);
                _userConnMap.erase(it);
                break;
            }
        }
    }
    //设置用户状态
    if(user.getId() != -1)
    {
        user.setState("offline");
        _usermodel.updateState(user);
    }
}

//服务器异常终止，接到ctrl+c,重置状态
void ChatService::reset()
{
    _usermodel.resetState();
}

//一对一聊天服务
void ChatService::oneChat(const TcpConnectionPtr &conn, json &js, Timestamp time)
{
    int toid = js["toid"].get<int>();
    {
        std::unique_lock<std::mutex> lock(_connMutex);
        auto it = _userConnMap.find(toid);
        if(it != _userConnMap.end())
        {
            // toid在线，转发消息   服务器主动推送消息给toid用户
            it->second->send(js.dump());
            return;
        }
    }

    _offlinMsgModel.insert(toid,js.dump());//存储离线消息
}

//添加好友义务
void ChatService::addFriend(const TcpConnectionPtr &conn, json &js, Timestamp time)
{
    int userid = js["id"].get<int>();
    int friendid = js["friendid"].get<int>();

    // 存储好友信息
    _friendModel.insert(userid, friendid);
}

//创建群组
void ChatService::createGroup(const TcpConnectionPtr &conn, json &js, Timestamp time)
{
    int userid = js["id"].get<int>();
    std::string name = js["groupname"];
    std::string desc = js["groupdesc"];

    //储存新创建的群组消息
    Group group(-1,name,desc);
    if(_groupModel.createGroup(group))
    {
        //储存群组联系人信息
        _groupModel.addGroup(userid,group.getId(),"creator");
    }
}

//加入群组业务
void ChatService::addGroup(const TcpConnectionPtr &conn, json &js, Timestamp time)
{
    int userid = js["id"].get<int>();
    int groupid = js["groupid"].get<int>();
    _groupModel.addGroup(userid,groupid,"normal");
}

//群组聊天业务
void ChatService::groupChat(const TcpConnectionPtr &conn, json &js, Timestamp time)
{
    int userid = js["id"].get<int>();
    int groupid = js["groupid"].get<int>();
    std::vector<int> uservec = _groupModel.queryGroupUsers(userid,groupid);
    std::unique_lock<std::mutex> lock(_connMutex);

    for(int id : uservec)
    {
        auto it = _userConnMap.find(id);
        if(it != _userConnMap.end())
        {
            //转发群消息
            it->second->send(js.dump());
        }else
        {
            _offlinMsgModel.insert(userid,js.dump());
        }
    }
}

//处理注销义务
void ChatService::loginout(const TcpConnectionPtr &conn, json &js, Timestamp time)
{
    int userid = js["id"].get<int>();
    {
        std::unique_lock<std::mutex> lock(_connMutex);
        auto it = _userConnMap.find(userid);
        if(it != _userConnMap.end())
        {
            _userConnMap.erase(it);
        }
    }
    User user(userid,"","","offline");
    _usermodel.updateState(user);
}