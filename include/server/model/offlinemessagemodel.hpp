/*
 * @Author: 快出来了哦
 * @Date: 2023-09-22 12:43:43
 * @LastEditors: 快出来了哦
 * @LastEditTime: 2023-09-22 12:53:06
 * @FilePath: /chatserver/include/server/model/offlinemessagemodel.hpp
 * @Description: 
 */
#ifndef OFFLINEMESSAGEMODEL_H
#define OFFLINEMESSAGEMODEL_H

#include <string>
#include <vector>
//提供离线消息表的操纵方法
class OfflinemsgModel
{
public:
    //储存用户离线消息
    void insert(int userid,std::string msg);
    //删除用户离线消息
    void remove(int userid);
    //查询用户的离线消息
    std::vector<std::string> query(int userid);

};
#endif