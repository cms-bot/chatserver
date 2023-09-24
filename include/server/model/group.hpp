/*
 * @Author: 快出来了哦
 * @Date: 2023-09-24 14:16:52
 * @LastEditors: 快出来了哦
 * @LastEditTime: 2023-09-24 14:17:59
 * @FilePath: /chatserver/include/server/model/group.hpp
 * @Description: 
 */
#ifndef GROUP_H
#define GROUP_H

#include "groupuser.hpp"
#include <string>
#include <vector>

// User表的ORM类
class Group
{
public:
    Group(int id = -1, std::string name = "", std::string desc = "")
    {
        this->id = id;
        this->name = name;
        this->desc = desc;
    }

    void setId(int id) { this->id = id; }
    void setName(std::string name) { this->name = name; }
    void setDesc(std::string desc) { this->desc = desc; }

    int getId() { return this->id; }
    std::string getName() { return this->name; }
    std::string getDesc() { return this->desc; }
    std::vector<GroupUser> &getUsers() { return this->users; }

private:
    int id;
    std::string name;
    std::string desc;
    std::vector<GroupUser> users;
};

#endif