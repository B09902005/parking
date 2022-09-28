#include <string>
#include <iostream>
#include "cell.hpp"

#define pi 3.1415926

// initialize parameter
Cell::Cell(int _x, int _y){
    this->posx = _x;
    this->posy = _y;
    this->car = nullptr;
}

Car::~Car(){}
