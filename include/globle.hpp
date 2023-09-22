/*
 * @Author: 快出来了哦
 * @Date: 2023-09-20 14:06:14
 * @LastEditors: 快出来了哦
 * @LastEditTime: 2023-09-21 14:25:57
 * @FilePath: /chatserver/include/globle.hpp
 * @Description: 
 */
#ifndef GLOBLEHPP_H
#define GLOBLEHPP_H

enum EnMsgType
{
    LOGIN_MSG = 1, // 登录消息
    LOGIN_MSG_ACK, // 登录响应消息
    LOGINOUT_MSG, // 注销消息
    REG_MSG, // 注册消息
    REG_MSG_ACK, // 注册响应消息
    ONE_CHAT_MSG, // 聊天消息
    ADD_FRIEND_MSG, // 添加好友消息

    CREATE_GROUP_MSG, // 创建群组
    ADD_GROUP_MSG, // 加入群组
    GROUP_CHAT_MSG, // 群聊天
};
#endif