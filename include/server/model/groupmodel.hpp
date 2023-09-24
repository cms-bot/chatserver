/*
 * @Author: 快出来了哦
 * @Date: 2023-09-24 14:17:24
 * @LastEditors: 快出来了哦
 * @LastEditTime: 2023-09-24 14:17:28
 * @FilePath: /chatserver/include/server/model/groupmodel.hpp
 * @Description: 
 */
#ifndef GROUPMODEL_H
#define GROUPMODEL_H

#include "group.hpp"
#include <string>
#include <vector>
using namespace std;

// 维护群组信息的操作接口方法
class GroupModel
{
public:
    // 创建群组
    bool createGroup(Group &group);
    // 加入群组
    void addGroup(int userid, int groupid, string role);
    // 查询用户所在群组信息
    std::vector<Group> queryGroups(int userid);
    // 根据指定的groupid查询群组用户id列表，除userid自己，主要用户群聊业务给群组其它成员群发消息
    std::vector<int> queryGroupUsers(int userid, int groupid);
};

#endif