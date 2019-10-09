//Server.cpp
#include <iostream>
#include <unistd.h>
#include "server.h"
#include <string>
#include <algorithm>


using namespace std;

// 服务端类构造函数
MyServer::MyServer(){

    // 初始化服务器地址和端口
    serverAddr.sin_family = PF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP);

    // 初始化socket
    listener = 0;

    // epool fd
    epfd = 0;
}
// 初始化服务端并启动监听
void MyServer::Init() {
    cout << "Init Server..." << endl;

     //创建监听socket
    listener = socket(PF_INET, SOCK_STREAM, 0);
    if(listener < 0) { perror("listener"); exit(-1);}

    // 设置套接字选项避免 端口占用错误
    int on=1;
    if((setsockopt(listener,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on)))<0)
    {
        perror("set sockopt failed");
        exit(EXIT_FAILURE);
    }

    //绑定地址
    if( bind(listener, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("bind error");
        exit(-1);
    }

    //监听
    int ret = listen(listener, 5);
    if(ret < 0) {
        perror("listen error");
        exit(-1);
    }

    cout << "Start to listen: " << SERVER_IP << endl;

    //在内核中创建事件表 epfd是一个句柄
    epfd = epoll_create (EPOLL_SIZE);

    if(epfd < 0) {
        perror("epfd error");
        exit(-1);
    }

    //往事件表里添加监听事件
    this->addfd(epfd, listener, true);

}

// 注册新的fd到epollfd中
// 参数enable_et表示是否启用ET模式，如果为True则启用，否则使用LT模式
void MyServer::addfd( int epollfd, int fd, bool enable_et )
{
    struct epoll_event ev;
    ev.data.fd = fd;
    ev.events = EPOLLIN;
    if( enable_et )
        ev.events = EPOLLIN | EPOLLET;
    epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &ev);
    // 设置socket为非阻塞模式
    // 套接字立刻返回，不管I/O是否完成，该函数所在的线程会继续运行
    //eg. 在recv(fd...)时，该函数立刻返回，在返回时，内核数据还没准备好会返回WSAEWOULDBLOCK错误代码
    fcntl(fd, F_SETFL, fcntl(fd, F_GETFD, 0)| O_NONBLOCK);
    printf("fd added to epoll!\n\n");
}

// 启动服务端
void MyServer::Start() {

    // epoll 事件队列
    static struct epoll_event events[EPOLL_SIZE];

    // 初始化服务端
    Init();
    // 初始化食物
    my_map.init_food();
    // 启动蛇的移动定时器
    t.StartTimer(300, std::bind(this->SendMessage,&snake_list,&my_map));

    std::cout << "chu shi hua wuan cheng"<< std::endl;
    //主循环
    while(1)
    {
        //epoll_events_count表示就绪事件的数目
        int epoll_events_count = epoll_wait(epfd, events, EPOLL_SIZE, -1);

        if(epoll_events_count < 0) {
            perror("epoll failure");
            break;
        }

        //处理这epoll_events_count个就绪事件
        for(int i = 0; i < epoll_events_count; ++i)
        {
            int sockfd = events[i].data.fd;
            //新用户连接
            if(sockfd == listener)
            {
                // 建立 accept 连接  
                struct sockaddr_in client_address;
                socklen_t client_addrLength = sizeof(struct sockaddr_in);
                int clientfd = accept( listener, (struct sockaddr*)&client_address, &client_addrLength );

                this->addfd(epfd, clientfd, true);

                // 服务端用list保存用户连

                // 获取 客户端ip 
                char IP[32];
                inet_ntop(AF_INET,&client_address.sin_addr,IP,32);
                // 创建一条新蛇
                Snake *new_snake = new Snake(IP,clientfd);
                // 在地图上设置蛇的位置
                my_map.set_snake_head(new_snake);
                // 将蛇加入蛇链表
                snake_list.push_back(new_snake);
                printf("已与 %s 建立连接 %u\n",IP,clientfd);

                //  发送客户端初始化 json消息
                char message[BUF_SIZE];
                bzero(message, BUF_SIZE);
                string mes_str="{";
                // 蛇的名字
                mes_str.append("\"snake_name\":\"" + new_snake->snake_name + "\",");
                // 地图大小
                mes_str.append("\"MAP_WIDTH\":"+to_string(MAP_WIDTH + 1)+",");
                mes_str.append("\"MAP_HEIGHT\":"+to_string(MAP_HEIGHT +1)+",");
                // 客户端大小
                mes_str.append("\"CLIENT_WIDTH\":"+to_string(CLIENT_WIDTH)+",");
                mes_str.append("\"CLIENT_HEIGHT\":"+to_string(CLIENT_HEIGHT)+",");
                // 客户端缩放大小
                mes_str.append("\"CLIENT_STEP\":"+to_string(CLIENT_STEP)+",");
                // 根据不同的类型  客户端显示不同的 图片
                mes_str.append("\"1\":\"food_s.png\",");
                mes_str.append("\"2\":\"food_b.png\",");
                mes_str.append("\"3\":\"head.png\",");
                mes_str.append("\"4\":\"head_left.png\",");
                mes_str.append("\"5\":\"body.png\",");
                mes_str.append("\"map\":\"tile_map.png\"");
                mes_str.append("}");
                strcpy(message, mes_str.c_str());
                long ret = send(clientfd, message, BUF_SIZE, 0);
                if(ret < 0) {
                    perror("send error");
                    Close();
                    exit(-1);
                }
                printf("客户端初始化 消息已发送\n");
            }
            //处理用户发来的消息，并广播，使其他用户收到信息
            else {
                // 判断客户端 是那条蛇， 并接收消息
                for(list<Snake*>::iterator iter_find = snake_list.begin();iter_find != snake_list.end();iter_find++){
                    if((*iter_find)->client_fd == sockfd){
                        this->ReceiveMessage(*iter_find);
                        break;
                    }
                }
            }
        }
    }

    // 关闭服务
    Close();
}

// 关闭服务，清理并关闭文件描述符
void MyServer::Close() {

    //关闭socket
    close(listener);

    //关闭epoll监听
    close(epfd);

    // 退出 蛇的移动  定时器
    t.Expire();
}
// 接收客户端消息
void MyServer::ReceiveMessage(Snake *my_snake){
    char recv_buf[BUF_SIZE];
    bzero(recv_buf, BUF_SIZE);
    int len = recv(my_snake->client_fd, recv_buf, BUF_SIZE, 0);
    // 如果客户端关闭了连接
    if(len == 0){
        std::cout << "客户端关闭 " << std::endl;
        my_snake->is_die = true;
    }else{
        // 根据客户端发来的消息 设置蛇的方向
        string recv_str = recv_buf;
        transform(recv_str.begin(), recv_str.end(), recv_str.begin(), ::toupper);
        if(recv_str == "KEY_UP"){
            my_snake->snake_d = key_up;
        }else if(recv_str == "KEY_DOWN"){
            my_snake->snake_d = key_Down;
        }else if(recv_str == "KEY_LEFT"){
            my_snake->snake_d = key_Left;
        }else if(recv_str == "KEY_RIGHT"){
            my_snake->snake_d = key_Right;
        }
    }
}
// 定时器触发的  发送消息函数， 1. 蛇的移动  2. 删除蛇  3. 发送 最新地图状态到 客户端
void MyServer::SendMessage(list<Snake*> *snake_list, MyMap *my_map){
    char send_buf[BUF_SIZE];
    bzero(send_buf, BUF_SIZE);
    string send_str;
    // 蛇的移动
    for(list<Snake*>::iterator iter = snake_list->begin();iter != snake_list->end();iter++){
        (*iter)->move(my_map);
    }
    // 从蛇链表中删除蛇  并做清理操作
    snake_list->remove_if([my_map](Snake *snake){
        if(snake->is_die){
            // 将蛇的身体变成实物
            snake->die(my_map);
            // 关闭蛇的客户端连接
            close(snake->client_fd);
            // 删除蛇对象
            delete snake;
            return true;
        }
        return false;
    });
    // 将蛇 和 周边状态  发送给客户端
    for(list<Snake*>::iterator iter = snake_list->begin();iter != snake_list->end();iter++){
        // 获取蛇的头部所在区域
        Space *snake_head = (*iter)->snake_body_arr.front();
        int half_width = CLIENT_WIDTH / 2;
        int half_height = CLIENT_HEIGHT / 2;
        std::cout << snake_head->map_x - half_width << "|"<< snake_head->map_y - half_height << "|" <<  snake_head->map_x + half_width << "|" << snake_head->map_y + half_height << "|"  << std::endl;
        list<Space> part_map = my_map->get_part_map(snake_head->map_x - half_width, snake_head->map_y - half_height,snake_head->map_x + half_width, snake_head->map_y + half_height);
        // 根据该区域 各格子的内容组装json 消息发送给客户端
        send_str = "";
        int type;
        int is_me;
        send_str.append("[");
        for(list<Space>::iterator iter_v = part_map.begin();iter_v != part_map.end();iter_v++){
            is_me = 0;
            if(iter_v->space_t == food){
                if(iter_v->food->food_size == FOOD_SMALL_SIZE){
                    // 小食物
                    type = 1;
                }else{
                    // 大食物
                    type = 2;
                }
            }else if(iter_v->space_t == snake){
                Space *new_space = iter_v->snake->snake_body_arr.front();
                if(new_space->map_x == iter_v->map_x && new_space->map_y == iter_v->map_y){
                    if(iter_v->snake->snake_d == key_up || iter_v->snake->snake_d == key_Down){
                        // 向上和向下的头部图片
                       type = 3;
                    }else{
                        // 向左和向右的头部图片
                       type = 4;
                    }
                    if(snake_head->map_x == iter_v->map_x && snake_head->map_y == iter_v->map_y){
                        // 如果这个头部是自己而不是别的蛇   就返回1
                        is_me = 1;
                    }
                }else{
                    // 蛇的身体图片
                    type = 5;
                }
            }else{
                type = 0;
            }
            send_str.append("{\"m\":"+  to_string(is_me) +",");
            send_str.append("\"t\":"+  to_string(type) +",");
            send_str.append("\"x\":"+  to_string(iter_v->map_x) +",");
            send_str.append("\"y\":"+  to_string(iter_v->map_y) +"},");
        }
        send_str.pop_back();
        send_str.append("]");
        strcpy(send_buf, send_str.c_str());
        send((*iter)->client_fd,send_buf, sizeof(send_buf), 0);
    }
}




