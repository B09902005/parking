#include <string>
#include <iostream>
#include "car.hpp"
#include <cmath>

#define pi 3.1415926

// initialize parameter
Car::Car(float _x, float _y, std::string ID, std::string path, int w, int h){
    this->x = _x;
    this->y = _y;
    this->speed = 0.2;
    this->angle = 180;
    this->ID = ID;
    this->img = ImageProcess::load_bitmap_at_size(path.c_str(), w, h);
    if (!this->img)
        LOG::game_abort("failed to load image: object");
}

Car::Car(float _x, float _y, std::string ID, ALLEGRO_BITMAP *_img){
    this->x = _x;
    this->y = _y;
    this->speed = 0.2;
    this->angle = 180;
    this->wheel = 0;
    this->ID = ID;
    this->img = _img;
}

Car::~Car(){
    al_destroy_bitmap(this->img);
}

bool Car::update() {
	this->x += this->speed * cos(angle * pi / 180);
	this->y -= this->speed * sin(angle * pi / 180);
    angle += wheel;
    if (this->x < 20) this->wheel = 1;
    if (this->angle >= 270) this->wheel = 0;
    if ((this->x <= 0) or (this->y <= 0) or (this->y >= space_height)) return false;
	return true;
    // if exit then return false
}


