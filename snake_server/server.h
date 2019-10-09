#ifndef MYSERVER_H
#define MYSERVER_H

#include <iostream>
#include <list>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "config.h"
#include "snake.h"
#include "Timer.hpp"

using namespace std;

// 其他用户收到消息的前缀
#define SERVER_MESSAGE "ClientID %d say >> %s"
#define SERVER_PRIVATE_MESSAGE "Client %d say to you privately >> %s"
#define SERVER_PRIVATE_ERROR_MESSAGE "Client %d is not in the chat room yet~"
// 提醒你是聊天室中唯一的客户
#define CAUTION "There is only one int the char room!"
// 退出系统
#define EXIT "EXIT"

//定义信息结构，在服务端和客户端之间传送
struct Msg
{
    int type;
    int fromID;
    int toID;
    char content[BUF_SIZE];

};



struct server_msg{
    char name[32];
    int client_width = CLIENT_WIDTH;
    int client_height = CLIENT_HEIGHT;

};
struct client_msg{
    char snake_direction[10];
};


class MyServer
{
public:

    // 无参数构造函数
    MyServer();

    // 初始化服务器端设置
    void Init();

    // 关闭服务
    void Close();

    // 启动服务端
    void Start();

private:

    // 服务器端serverAddr信息
    struct sockaddr_in serverAddr;

    //创建监听的socket
    int listener;

    // 客户端列表
    // epoll_create创建后的返回值
    int epfd;

    void addfd(int epollfd, int fd, bool enable_et);
    // 我的地图
    MyMap my_map;

    // 蛇链表
    list<Snake*> snake_list;

    // 发送消息
    static void SendMessage(list<Snake*> *snake_list, MyMap *my_map);
    // 接收消息
    void ReceiveMessage(Snake *my_snake);
    // 在网上找到  定时器的实现
    MyTimer t;

};

#endif // MYSERVER_H
