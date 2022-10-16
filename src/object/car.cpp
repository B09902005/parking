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
    this->angle = 180;
    this->brake_timer = 0;
    this->wheel_timer = 0;
    this->ID = ID;
    this->cell = std::make_pair(-1,-1);
    this->state = 0;
    this->priority = 1;
    this->img = ImageProcess::load_bitmap_at_size(path.c_str(), w, h);
    if (!this->img)
        LOG::game_abort("failed to load image: object");
}

Car::Car(float _x, float _y, std::string ID, ALLEGRO_BITMAP *_img){
    this->x = _x;
    this->y = _y;
    this->angle = 180;
    this->brake_timer = 0;
    this->wheel_timer = 0;
    this->ID = ID;
    this->cell = std::make_pair(-1,-1);
    this->state = 0;
    this->priority = 1;
    this->img = _img;
}

Car::~Car(){
    al_destroy_bitmap(this->img);
}

std::pair<int, int> approaching_block(Car *car){
    // if near a block then return 1 (speed down)
    // if even nearer then return 2 (turn wheel)
    std::vector <int> blockx{2,7,12};
    for (auto i: blockx){
        float distance = car->x - (width/30.0 + i*width/15.0);
        if ((car -> angle + 1080) % 360 == 180){
            if ((distance <= 5) and (distance > 4.8)) return std::make_pair(1,i);
            if ((distance <= 2.9) and (distance > 2.7)) return std::make_pair(2,i);
        }else if ((car -> angle + 1080) % 360 == 0){
            if ((distance >= -5) and (distance < -4.8)) return std::make_pair(1,i);
            if ((distance >= -2.9) and (distance < -2.7)) return std::make_pair(2,i);
        }
    }
    std::vector <int> blocky{0,7,14};
    if (car -> ID == "GUB-450") LOG::game_log("distance: %f %f %d", car ->y, height/30.0 + 14*height/15.0, car ->angle);
    for (auto i: blocky){
        float distance = car->y - (height/30.0 + i*height/15.0);
        if ((car -> angle + 1080) % 360 == 90){
            if ((distance <= 5) and (distance > 4.8)) return std::make_pair(1,i);
            if ((distance <= 2.9) and (distance > 2.7))
                return std::make_pair(2,i);
        }else if ((car -> angle + 1080) % 360 == 270){
            if ((distance >= -5) and (distance < -4.8)) return std::make_pair(1,i);
            if ((distance >= -2.9) and (distance < -2.7))
                return std::make_pair(2,i);
        }
    }
    return std::make_pair(0,-1);
}

std::pair<int, int> approaching_cell(Car *car){
    // if too over then return -1 (go back)
    // if still far then return 0 (continue)
    // if near the destination then return 1 (speed down)
    // if even nearer then return 2 (left wheel) or 3 (right wheel)
    // if arrive then reture 4 (stop)
    if (car -> state >= 3) return std::make_pair(0,-1);
    float distance = car->y - (height/30.0 + (car->cell.second)*height/15.0);
    if ((car -> angle + 1080) % 360 == 90){
        if ((distance <= 5) and (distance > 4.8)) return std::make_pair(1,-1);
        if ((distance <= 2.9) and (distance > 2.8)){
            if (car->cell.first % 5 < 2) return std::make_pair(2,-1);
            if (car->cell.first % 5 > 2) return std::make_pair(3,-1);
        }
    }else if ((car -> angle + 1080) % 360 == 270){
        if ((distance >= 3) and (distance < 3.2)) return std::make_pair(-1, -1);
        if ((distance >= 3.5) and (distance < 3.6)) return std::make_pair(-2, -1);
        if ((distance >= -5) and (distance < -4.8)) return std::make_pair(1,-1);
        if ((distance >= -2.9) and (distance < -2.8)){
            if (car->cell.first % 5 > 2) return std::make_pair(2,-1);
            if (car->cell.first % 5 < 2) return std::make_pair(3,-1);
        }
    }else if (((car -> angle + 1080) % 360 == 0) or ((car -> angle + 1080) % 360 == 180)){
        if (abs(distance) > height / 30.0) return std::make_pair(0,-1);
        distance = abs((width/30.0 + (car->cell.first)*width/15.0) - car->x);
        if ((distance <= 2) and (distance > 1.8)) return std::make_pair(1,-1);
        if (distance <= 0.1) return std::make_pair(4,-1);
    }
    return std::make_pair(0,-1);
}

bool getroad(Car *car){
    int x = car -> cell.first / 5 * 5 + 2;
    int y = 0;
    if ((car -> state >= 3) and (car -> cell.second > 7)) y = 1;
    if ((car -> angle + 1080) % 360 == 270) y = 1;
    if (road[x][y] == nullptr) road[x][y] = car;
    else if (road[x][y] != car) car -> priority = 2;
    return (road[x][y] == car);
}

bool Car::update() {
    if (this -> priority == 2) return true; // waiting car in the front.
    if (this -> state == 2){ // stopping at a cell.
        int probability_inverse = 13000;
        if (rand() % probability_inverse == 0){
            this -> state = 3;
            this -> priority = 1;
            if (this -> cell.first % 5 < 2) this -> angle = 0;
            if (this -> cell.first % 5 > 2) this -> angle = 180;
        }
        return true;
    }
    if ((this -> state == 0) and (this -> x > width) and (this -> x < width + 0.2)) return true; // the parking lot is full.
    
    std::pair <int,int> approaching = approaching_block(this);
    if (this -> ID == "GUB-450") LOG::game_log("approach: %d", approaching.first);
    if (approaching.first == 1) brake_timer = 40; // slow down if near a block.
    if (approaching.first == 2){ // decide direction if near a block.
        if (((this -> angle + 1080) % 360 == 180) and (abs(approaching.second - this->cell.first) <= 2)){
            if ((this -> state >= 3) and (this -> cell.second % 7 >= 4)) this -> wheel_timer = -90;
            else this -> wheel_timer = 90;
            getroad(this);
        }else if (((this -> angle + 1080) % 360 == 0) and (this -> y < height / 15.0 * 14.0)){
            if (this -> cell.second % 7 >= 4) this -> wheel_timer = 90;
            else this -> wheel_timer = -90;
            getroad(this);
        }else if ((this -> angle + 1080) % 360 == 270){
            int roadnum = this -> cell.first / 5 * 5 + 2;
            if (approaching.second == 14){
                this -> wheel_timer = 90;
                road[roadnum][1] = nullptr;
            }
            else{
                road[roadnum][0] = nullptr;
                getroad(this);
            }
        }
    }
    
    approaching = approaching_cell(this);
    int roadnum = this -> cell.first / 5 * 5 + 2;
    if (approaching.first == 1) this -> brake_timer = 20; // slow down if near a cell.
    if (approaching.first == 2){ // turn right
        if (this->cell.second < 7) road[roadnum][0] = nullptr;
        if (this->cell.second > 7) road[roadnum][1] = nullptr;
        this -> wheel_timer = 90;
        this -> priority = 0;
    }
    if (approaching.first == 3){ // turn left
        if (this->cell.second < 7) road[roadnum][0] = nullptr;
        if (this->cell.second > 7) road[roadnum][1] = nullptr;
        this -> wheel_timer = -90;
        this -> priority = 0;
    }
    if (approaching.first == 4){ // stop
        this -> state = 2;
        this -> brake_timer = 0;
        this -> wheel_timer = 0;
        this -> priority = 1;
    }
    if (approaching.first == -1) this -> brake_timer += 40;
    if (approaching.first == -2){
        this -> angle += 180;
        this -> priority = 0;
    }
    if ((this -> state == 4) and ((this -> angle + 1080) % 360 == 90)) this -> angle += 180;
    
    if (this -> priority == 2) return true;
    
    // let the car moves.
    float speed = 0.2;
    if (this->wheel_timer != 0) speed = 0.05;
    else if (this->brake_timer > 0){
        speed = 0.1;
        brake_timer --;
    }
	this->x += speed * cos(angle * pi / 180);
	this->y -= speed * sin(angle * pi / 180);
    
    if (this -> wheel_timer > 0){
        this -> angle ++;
        this -> wheel_timer --;
    }
    if (this -> wheel_timer < 0){
        this -> angle --;
        this -> wheel_timer ++;
    }
    
    if ((this->x <= 0) or (this->y <= 0) or (this->y >= height) or (this->x >= width*1.4)) return false;
	return true;
}


