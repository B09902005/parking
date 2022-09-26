#include <string>
#include <iostream>
#include "car.hpp"

// initialize parameter
Car::Car(float _x, float _y, std::string ID, std::string path, int w, int h){
    this->x = _x;
    this->y = _y;
    this->speedX = 0;
    this->speedY = 0;
    this->angle = 0;
    this->ID = ID;
    this->img = ImageProcess::load_bitmap_at_size(path.c_str(), w, h);
    if (!this->img)
        LOG::game_abort("failed to load image: object");
}

Car::Car(float _x, float _y, std::string ID, ALLEGRO_BITMAP *_img){
    this->x = _x;
    this->y = _y;
    this->speedX = 0;
    this->speedY = 0;
    this->angle = 0;
    this->ID = ID;
    this->img = _img;
}

Car::~Car(){
    al_destroy_bitmap(this->img);
}

bool Car::update() {
	this->x += this->speedX;
	this->y += this->speedY;
	//this->speedX = 0;
	//this->speedY = 0;
	return true;
}


