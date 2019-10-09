#ifndef SNAKE_H
#define SNAKE_H

#include "config.h"
#include <string>
#include "map.h"
#include <list>
using namespace std;

class Snake
{
public:
    Snake(string snake_name,int client_fd, int eat_food = 0, bool is_die = false, int snake_size = SNAKE_SIZE,snake_direction snake_d=key_up);
    // 蛇的名字
    string snake_name;
    // 客户端的 fd 
    int client_fd;
    // 蛇的长度
    int snake_size;
    // 吃的食物分数（不够一节蛇身体的部分）
    int eat_food;
    //  是否死掉了
    bool is_die;
    // 蛇的方向
    snake_direction snake_d;
    // 保存蛇身体的列表
    list<Space*> snake_body_arr;
    // 蛇的移动方法
    void move(MyMap *my_map);
    // 蛇吃食物的方法
    void eat(MyMap *my_map,Space *space);
    // 蛇死掉的方法
    bool die(MyMap *my_map);

};

#endif // SNAKE_H
