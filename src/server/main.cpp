/*
 * @Author: 快出来了哦
 * @Date: 2023-09-20 15:56:04
 * @LastEditors: 快出来了哦
 * @LastEditTime: 2023-09-21 19:16:45
 * @FilePath: /chatserver/src/server/main.cpp
 * @Description: 
 */
#include "chatserver.hpp"
#include "chatservice.hpp"

#include <iostream>
#include <signal.h>

void restHandler(int)
{
    ChatService::instance()->reset();
    exit(0);
}
int main()
{
    signal(SIGINT,restHandler);
    EventLoop loop;
    InetAddress addr("127.0.0.1",6000);
    ChatServer server(&loop,addr,"chatserver");
    server.start();
    loop.loop();
    return 0;
}