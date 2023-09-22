/*
* @Author: 快出来了哦
* @Date: 2023-09-22 12:54:21
 * @LastEditors: 快出来了哦
 * @LastEditTime: 2023-09-22 13:04:20
 * @FilePath: /chatserver/src/server/model/offlinemessagemodel.cpp
* @Description: 
*/

#include "offlinemessagemodel.hpp"
#include "db.h"

//储存用户离线消息
void OfflinemsgModel::insert(int userid,std::string msg)
{
    char sql[1024] = {0};
    sprintf(sql, "insert into offlinemessage values(%d, '%s')", userid, msg.c_str());

    MySQL mysql;
    if(mysql.connect())
    {
        mysql.update(sql);
    }
}
//删除用户离线消息
void OfflinemsgModel::remove(int userid)
{
    // 1.组装sql语句
    char sql[1024] = {0};
    sprintf(sql, "delete from offlinemessage where userid=%d", userid);

    MySQL mysql;
    if (mysql.connect())
    {
        mysql.update(sql);
    }
}
//查询用户的离线消息
std::vector<std::string> OfflinemsgModel::query(int userid)
{
    // 1.组装sql语句
    char sql[1024] = {0};
    sprintf(sql, "select message from offlinemessage where userid = %d", userid);

    std::vector<std::string> vec;
    MySQL mysql;
    if (mysql.connect())
    {
        MYSQL_RES *res = mysql.query(sql);
        if (res != nullptr)
        {
            // 把userid用户的所有离线消息放入vec中返回
            MYSQL_ROW row;
            while((row = mysql_fetch_row(res)) != nullptr)
            {
                vec.push_back(row[0]);
            }
            mysql_free_result(res);
            return vec;
        }
    }
    return vec;
}