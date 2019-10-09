#ifndef MAP_H
#define MAP_H
using namespace std;

#include "food.h"
#include "config.h"
#include <list>

// 注意如果在这里引入 snake.h  会导致  snake 和  map 头文件的 相互引用 所以这里放一个空类 用于骗过编译器，真正的头文件放到  map.cpp
class Snake;
// 地图的格子  
struct Space
{
    space_type space_t = empty;
    Food *food;
    Snake *snake;
    int map_x;
    int map_y;
};

// 地图类
class MyMap
{
public:
    MyMap();
    // 方法的具体作用 请查看map.cpp
    void init_food();
    void set_food(list<Space*> *food_list, int food_size = FOOD_BIG_SIZE);
    void del_food(Space space);
    void set_snake_head(Snake *new_snake);
    void set_snake_body(Snake *new_snake, Space space);
    void del_snake_body(Snake *snake);
    Space *get_space(int map_x, int map_y);
    list<Space> get_part_map(int left, int top, int right, int bottom);
    list<Space*> surplus_space;
    Space map_arr[MAP_WIDTH][MAP_HEIGHT];
};

#endif // MAP_H
