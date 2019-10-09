#ifndef FOOD_H
#define FOOD_H

#include <string>
using namespace std;

#include "config.h"

class Food
{
public:
    Food(int food_size=FOOD_SMALL_SIZE);
    int food_size;
};
#endif // FOOD_H
