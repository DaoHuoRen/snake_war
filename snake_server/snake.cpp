#include "snake.h"
#include <iostream>

// 蛇的构造
Snake::Snake(string snake_name,int client_fd, int eat_food, bool is_die, int snake_size,snake_direction snake_d){
    this->snake_name = snake_name;
    this->client_fd = client_fd;
    this->eat_food = eat_food;
    this->is_die=is_die;
    this->snake_size = snake_size;
    this->snake_d = snake_d;
}
// 蛇的移动
void Snake::move(MyMap *my_map){
    // 获取蛇的头部
    int new_space_x = this->snake_body_arr.front()->map_x;
    int new_space_y = this->snake_body_arr.front()->map_y;
    // 根据蛇的移动方向  找到下一个移动的位置
    if(this->snake_d == key_up){
        if(new_space_y > 0){
            new_space_y -= 1;
        }else{
            return;
        }

    }else if(this->snake_d == key_Down){
        if(new_space_y + 1 < MAP_HEIGHT){
            new_space_y ++;
        }else{
            return;
        }
    }else if(this->snake_d == key_Left){
        if(new_space_x > 0){
            new_space_x --;
        }else{
            return;
        }
    }else if(this->snake_d == key_Right){
        if(new_space_x + 1 < MAP_WIDTH){
            new_space_x ++;
        }else{
            return;
        }
    }
    // 获取地图上要移动上去的格子
    Space *new_space = my_map->get_space(new_space_x,new_space_y);
    if(new_space->space_t != snake || new_space->snake == this){
        // 如果是食物  则吃掉食物
        if(new_space->space_t == food){
            this->eat(my_map,new_space);
        }
        // 如果是 蛇身体列表的长度  小于 蛇的长度 ，表示还有蛇身体没有出现在地图上，则只 在头部加一节
        if(this->snake_body_arr.size() < this->snake_size){
            my_map->set_snake_body(this,*new_space);
        }else{  // 如果长度够了   则在头部加一节  在尾部减一节
            my_map->set_snake_body(this,*new_space);
            my_map->del_snake_body(this);
        }
    }else{
        this->is_die = true;
    }
    return;
}
// 蛇吃食物
void Snake::eat(MyMap *my_map, Space *space){
    this->eat_food += space->food->food_size;
    if(this->eat_food >= FOOD_BIG_SIZE){
        this->snake_size ++;
        this->eat_food -= FOOD_BIG_SIZE;
    }
    my_map->del_food(*space);
}
// 蛇死掉
bool Snake::die(MyMap *my_map){
    cout << "I am in die" << endl;
    my_map->set_food(&(this->snake_body_arr), FOOD_BIG_SIZE);
    return true;
}
