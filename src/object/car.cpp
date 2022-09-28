#include <string>
#include <iostream>
#include "car.hpp"
#include <cmath>
#include <vector>

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

int approaching_block(Car *car){
    // if near a block then return 1 (speed down)
    // if even nearer then return 2 (turn wheel)
    std::vector <int> blockx{0,7,14};
    for (auto i: blockx){
        float distance = car->x - (width/30.0 + i*width/15.0);
        if (car -> angle % 360 == 180){
            if ((distance <= 5) and (distance > 4.8)) return 1;
            if ((distance <= 2.9) and (distance > 2.8)) return 2;
        }else if (car -> angle % 360 == 0){
            if ((distance >= -5) and (distance < -4.8)) return 1;
            if ((distance >= -2.9) and (distance < -2.8)) return 2;
        }
    }
    std::vector <int> blocky{2,7,12};
    for (auto i: blocky){
        float distance = car->y - (height/30.0 + i*height/15.0);
        if (car -> angle % 360 == 90){
            if ((distance <= 5) and (distance > 4.8)) return 1;
            if ((distance <= 2.9) and (distance > 2.8)) return 2;
        }else if (car -> angle == 270){
            if ((distance >= -5) and (distance < -4.8)) return 1;
            if ((distance >= -2.9) and (distance < -2.8)) return 2;
        }
    }
    return 0;
}

bool Car::update() {
	this->x += this->speed * cos(angle * pi / 180);
	this->y -= this->speed * sin(angle * pi / 180);
    int approaching = approaching_block(this);
    if (approaching == 1) this->speed = 0.1;
    if (approaching == 2){
        this->speed = 0.05;
        this->wheel = rand()%3-1;
    }
    angle += wheel;
    if ((this->angle % 90 == 0) and (speed < 0.07)){
        this->speed = 0.2;
        this->wheel = 0;
    }
    if ((this->x <= 0) or (this->y <= 0) or (this->y >= height) or (this->x >= width+1)) return false;
	return true;
}


