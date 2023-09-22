/*
 * @Author: 快出来了哦
 * @Date: 2023-09-21 13:49:10
 * @LastEditors: 快出来了哦
 * @LastEditTime: 2023-09-21 19:21:58
 * @FilePath: /chatserver/include/server/model/usermodel.hpp
 * @Description: 
 */
#ifndef USERMODEL_H
#define USERMODEL_H

#include "user.hpp"

//user表中的数据操作类
class UserModel
{
public:
    //user表中增加放法
    bool insert(User& user);

    //更新用户的状态信息
    bool updateState(User user);

    //根据用户id查询用户
    User query(int id);

    //重置用户状态
    void resetState();

};
#endif