#ifndef CONFIG_H
#define CONFIG_H

// 默认服务器端IP地址
#define SERVER_IP "127.0.0.1"

// 服务器端口号
#define SERVER_PORT 8888

// int epoll_create(int size)中的size
// 为epoll支持的最大句柄数
#define EPOLL_SIZE 5000

// 缓冲区大小65535
#define BUF_SIZE 0xFFFF


// 地图宽度
#define MAP_WIDTH 300

// 地图高度
#define MAP_HEIGHT 150

// 客户端宽度
#define CLIENT_WIDTH 31

// 客户端高度
#define CLIENT_HEIGHT 15

// 客户端缩放大小 (地图的一个格子在客户端有多大)
#define CLIENT_STEP 30

// 蛇的初始 长度
#define SNAKE_SIZE 3

// 小食物 分数
#define FOOD_SMALL_SIZE 10
// 大食物 分数 （两个小食物 = 一个大食物 = 一节蛇身体）
#define FOOD_BIG_SIZE 20
// 初始化食物时  食物所占地图的比例
#define FOOD_PERCENT 0.2
// 地图格子的类型  空，食物，蛇
enum space_type {empty, food, snake};
// 上，下，左，右
enum snake_direction {key_up, key_Down, key_Left, key_Right};

#endif // CONFIG_H
