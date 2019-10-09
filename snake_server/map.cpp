#include "map.h"
#include "config.h"
#include <ctime>
#include <cstdlib>
#include <iterator>
// 注意如果吧 这条放到头文件 会导致  snake 和  map 头文件的 相互引用 所以只能放到这里
#include "snake.h"
#include <iostream>
using namespace std;
// 初始化地图 
MyMap::MyMap(){
    for(int i = 0; i < MAP_WIDTH;i++){
        for(int j = 0; j < MAP_HEIGHT;j++){
            map_arr[i][j].map_x = i;
            map_arr[i][j].map_y = j;
            surplus_space.push_back(&map_arr[i][j]);
        }
    }
}
// 食物初始化操作
void MyMap::init_food()
{
    srand((unsigned)time(0));
    int food_Numb = MAP_WIDTH * MAP_HEIGHT * FOOD_PERCENT;

    Space food_space;
    while (food_Numb > 0) {
        int r_w = rand() % MAP_WIDTH;
        int r_h = rand() % MAP_HEIGHT;
        if(this->map_arr[r_w][r_h].space_t == empty){
            this->map_arr[r_w][r_h].space_t = food;
            Food *food = new Food(FOOD_SMALL_SIZE);
            this->map_arr[r_w][r_h].food = food;
            surplus_space.remove(&map_arr[r_w][r_h]);
            --food_Numb;
        }
    }
// 测试食物初始化的内容
//    for(int i = 0; i < MAP_WIDTH;i++){
//        for(int j = 0; j < MAP_HEIGHT;j++){
//            cout << this->map_arr[i][j].space_t << "|";
//        }
//        cout << endl;
//    }
}
// 蛇死掉 变食物
void MyMap::set_food(list<Space*> *food_list, int food_size){
    // 注意 这里可能是最坑的部分  bug 极有可能在这里
    list<Space*>::iterator it;
    int this_index = 0;
    for (it = food_list->begin(); it != food_list->end(); it++)
    {
        Food *new_food = new Food(food_size);
        this->map_arr[(*it)->map_x][(*it)->map_y].space_t = food;
        this->map_arr[(*it)->map_x][(*it)->map_y].food = new_food;
        this_index ++;
        // 如果不使用 这个判断  这个迭代器不知为啥 会不断迭代下去  远远超过list 的 size ，至今不知为啥 
        // 欢迎大神填坑
        if(this_index >= food_list->size()){
            break;
        }
    }
}
// 删除食物
void MyMap::del_food(Space space){
    map_arr[space.map_x][space.map_y].space_t = empty;
    delete map_arr[space.map_x][space.map_y].food;
    surplus_space.push_back(&map_arr[space.map_x][space.map_y]);
}
// 随机在空的地图格子中 设置蛇的头部
void MyMap::set_snake_head(Snake *new_snake){
    srand((unsigned)time(0));
    int space_index = rand() % surplus_space.size();
    list<Space*>::iterator iter=surplus_space.begin();
    // 迭代器找到对应的位置
    advance(iter,space_index-1);
    Space space = **iter;
    cout << space.map_x << "|" <<  space.map_y << endl;
    this->map_arr[space.map_x][space.map_y].space_t = snake;
    this->map_arr[space.map_x][space.map_y].snake = new_snake;
    this->map_arr[space.map_x][space.map_y].map_x = space.map_x;
    this->map_arr[space.map_x][space.map_y].map_x = space.map_y;
    this->map_arr[space.map_x][space.map_y].snake->snake_body_arr.push_back(&map_arr[space.map_x][space.map_y]);
    surplus_space.remove(&map_arr[space.map_x][space.map_y]);

    cout << space.map_x << "|" <<  space.map_y << this->map_arr[space.map_x][space.map_y].space_t << endl;
}
// 设置蛇的身体，在蛇身体list 的头部添加一个
void MyMap::set_snake_body(Snake *new_snake, Space space){
    this->map_arr[space.map_x][space.map_y].space_t = snake;
    this->map_arr[space.map_x][space.map_y].snake = new_snake;
    this->map_arr[space.map_x][space.map_y].map_x = space.map_x;
    this->map_arr[space.map_x][space.map_y].map_y = space.map_y;
    this->map_arr[space.map_x][space.map_y].snake->snake_body_arr.push_front(&map_arr[space.map_x][space.map_y]);
    surplus_space.remove(&map_arr[space.map_x][space.map_y]);
}
// 删除蛇的身体，在蛇身体list 的头部删除一个
void MyMap::del_snake_body(Snake *snake){
    Space *space = snake->snake_body_arr.back();
    this->map_arr[space->map_x][space->map_y].space_t = empty;
    snake->snake_body_arr.pop_back();
    surplus_space.push_back(&map_arr[space->map_x][space->map_y]);
}
// 获取坐标位置的 地图格子  用于碰撞检查
Space *MyMap::get_space(int map_x,int map_y){
    return &(this->map_arr[map_x][map_y]);
}
// 截取地图的一部分  返回给客户端用于  显示客户端界面
list<Space> MyMap::get_part_map(int left, int top, int right, int bottom){
    if(left < 0){
        left = 0;
    }
    if(top < 0){
        top = 0;
    }
    if(right >= MAP_WIDTH){
        right = MAP_WIDTH;
    }
    if(bottom >= MAP_HEIGHT){
        right = MAP_HEIGHT;
    }
    list<Space> return_list;
    for(int i=left; i <= right; i++){
        for(int j=top; j <= bottom; j++){
            return_list.push_back(this->map_arr[i][j]);
        }
    }
    return return_list;
}
