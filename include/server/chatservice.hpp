/*
 * @Author: 快出来了哦
 * @Date: 2023-09-20 14:09:36
 * @LastEditors: 快出来了哦
 * @LastEditTime: 2023-09-24 14:30:17
 * @FilePath: /chatserver/include/server/chatservice.hpp
 * @Description: 
 */
#ifndef CHATSERVICE_H
#define CHATSERVICE_H

#include <muduo/net/TcpConnection.h>
#include <unordered_map>
#include <functional>
#include <string>
#include <mutex>
using namespace muduo;
using namespace muduo::net;

#include "usermodel.hpp"
#include "offlinemessagemodel.hpp"
#include "friendmodel.hpp"
#include "groupmodel.hpp"
#include "json.hpp"
using json = nlohmann::json;

//表示处理消息事件回调方法类型
typedef std::function<void(const TcpConnectionPtr&,json&,Timestamp)> MsgHandler;

//聊天服务器义务类
class ChatService
{
public:
    //获取单例对象
    static ChatService* instance();

    //处理登录业务
    void login(const TcpConnectionPtr &conn, json &js, Timestamp time);
    //处理注册业务
    void regist(const TcpConnectionPtr &conn, json &js, Timestamp time);
    //一对一聊天服务
    void oneChat(const TcpConnectionPtr &conn, json &js, Timestamp time);
    //添加好友义务
    void addFriend(const TcpConnectionPtr &conn, json &js, Timestamp time);
    //创建群组业务
    void createGroup(const TcpConnectionPtr &conn, json &js, Timestamp time);
    //加入群组业务
    void addGroup(const TcpConnectionPtr &conn, json &js, Timestamp time);
    //群组聊天业务
    void groupChat(const TcpConnectionPtr &conn, json &js, Timestamp time);
    //处理注销义务
    void loginout(const TcpConnectionPtr &conn, json &js, Timestamp time);
    //处理客户端异常退出
    void clientCloseException(const TcpConnectionPtr& conn);
    //服务器异常终止，接到ctrl+c,重置状态
    void reset();
    //获取消息对应的处理器
    MsgHandler getHandler(int msgid);

private:
    ChatService();

    //储存消息id及其业务处理方法
    std::unordered_map<int,MsgHandler> _msghandleMap;
    //保存客户端在线的连接
    std::unordered_map<int,TcpConnectionPtr> _userConnMap;
    //互斥锁，保证线程安全
    std::mutex _connMutex;

    //数据操作类对象
    UserModel _usermodel;
    OfflinemsgModel _offlinMsgModel;
    FriendModel _friendModel;
    GroupModel _groupModel;

};

#endif