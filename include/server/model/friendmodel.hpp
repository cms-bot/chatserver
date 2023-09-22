/*
 * @Author: 快出来了哦
 * @Date: 2023-09-22 12:44:23
 * @LastEditors: 快出来了哦
 * @LastEditTime: 2023-09-22 13:20:41
 * @FilePath: /chatserver/include/server/model/friendmodel.hpp
 * @Description: 
 */
#ifndef FRIENDMODEL_H
#define FRIENDMODEL_H

#include "user.hpp"
#include <vector>

//好友信息操作类
class FriendModel
{
public:
    //添加好友关系
    void insert(int userid,int friendid);
    //返回用户好友列表
    std::vector<User> query(int userid);
};
#endif