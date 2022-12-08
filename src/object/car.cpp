#include <string>
#include <iostream>
#include "car.hpp"
#include <cmath>
#include <vector>

#define pi 3.1415926

#define NOTHINGHAPPEN 0
#define SPEEDDOWN 1
#define TURNWHEEL 2
#define CONTINUE 0
#define TURNLEFTWHEEL 2
#define TURNRIGHTWHEEL 3
#define ROTATE180 -2
#define REACH 4

// initialize parameter
Car::Car(float _x, float _y, std::string ID, std::string path, int w, int h){
    this->x = _x;
    this->y = _y;
    this->angle = 900;
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
    this->angle = 900;
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
    /* 
    pair.first means operation, pair.second means affect by which block
    if near a block then return 1 (if need, speed down)
    if even nearer then return 2 (if need, turn wheel)
    */
    std::vector <int> blockx{2,7,12}; // 路口的x座標是第二個，第7個，第12個
    for (auto i: blockx){
        float distance = car->x - (width/30.0 + i*width/15.0); // car_x - 路口的 x 座標
        if (car -> angle % 360 == 180){ // face left
            if ((distance <= 5) and (distance > 4.8)) return std::make_pair(SPEEDDOWN,i);
            if ((distance <= 2.9) and (distance > 2.7)) return std::make_pair(TURNWHEEL,i);
        }else if (car -> angle % 360 == 0){ // face right
            if ((distance >= -5) and (distance < -4.8)) return std::make_pair(SPEEDDOWN,i);
            if ((distance >= -2.9) and (distance < -2.7)) return std::make_pair(TURNWHEEL,i);
        }
    }
    std::vector <int> blocky{0,7,14}; // 路口的y座標是第零個，第7個，第14個
    for (auto i: blocky){
        float distance = car->y - (height/30.0 + i*height/15.0);
        if (car -> angle % 360 == 90){ // face up
            if ((distance <= 5) and (distance > 4.8)) return std::make_pair(SPEEDDOWN,i);
            if ((distance <= 2.9) and (distance > 2.7)) return std::make_pair(TURNWHEEL,i);
        }else if (car -> angle % 360 == 270){ // face down
            if ((distance >= -5) and (distance < -4.8)) return std::make_pair(SPEEDDOWN,i);
            if ((distance >= -2.9) and (distance < -2.7)) return std::make_pair(TURNWHEEL,i);
        }
    }
    return std::make_pair(NOTHINGHAPPEN,-1); // nothing happen
}

std::pair<int, int> approaching_cell(Car *car){
    /*
    if too over then return -1 (go back)
    if still far then return 0 (continue)
    if near the destination then return 1 (speed down)
    if even nearer then return 2 (left wheel) or 3 (right wheel)
    if arrive then reture 4 (stop)
    */
    if (car -> state >= 3) return std::make_pair(CONTINUE,-1); // ready to go to exit, thus ignore
    float distance = car->y - (height/30.0 + (car->cell.second)*height/15.0);
    if (car -> angle % 360 == 90){ // face up
        if ((distance <= 5) and (distance > 4.8)) return std::make_pair(SPEEDDOWN,-1);
        if ((distance <= 2.9) and (distance > 2.8)){
            if (car->cell.first % 5 < 2) return std::make_pair(TURNLEFTWHEEL,-1); // turn left
            if (car->cell.first % 5 > 2) return std::make_pair(TURNRIGHTWHEEL,-1); // turn right
        }
    }else if (car -> angle % 360 == 270){ // face down
        if ((distance >= 3) and (distance < 3.2)) return std::make_pair(-1, -1);
        if ((distance >= 3.5) and (distance < 3.6)) return std::make_pair(ROTATE180, -1);
        if ((distance >= -5) and (distance < -4.8)) return std::make_pair(SPEEDDOWN,-1);
        if ((distance >= -2.9) and (distance < -2.8)){
            if (car->cell.first % 5 > 2) return std::make_pair(TURNLEFTWHEEL,-1);
            if (car->cell.first % 5 < 2) return std::make_pair(TURNRIGHTWHEEL,-1);
        }
    }else if ((car -> angle % 360 == 0) or (car -> angle % 360 == 180)){ // face left or face right
        if (abs(distance) > height / 30.0) return std::make_pair(CONTINUE,-1);
        distance = abs((width/30.0 + (car->cell.first)*width/15.0) - car->x);
        if ((distance <= 2) and (distance > 1.8)) return std::make_pair(SPEEDDOWN,-1);
        if (distance <= 0.1) return std::make_pair(REACH,-1);
    }
    return std::make_pair(NOTHINGHAPPEN,-1);
}

bool Car::getroad(){
    // car get right to enter conflict zone
    /*
    true: get right to enter conflict zone
    false: didn't get right to enter conflict zone
    */
    int a = -1, b = -1;
    if ((this -> y < height / 15.0) and (this -> angle % 360 == 180)){
        if (this -> wheel_timer == 0){
            if (this -> x < width * 2.0 / 3.0) a = 2;
            else if (this -> x < width) a = 7;
            else a = 12;
            b = 0;
        }else{
            if (this -> x < width / 3.0) a = 2;
            else if (this -> x < width * 2.0 / 3.0) a = 7;
            else a = 12;
            b = 1;
        }
    }else if ((abs(this -> y - height / 2.0) < 0.2) and (this -> angle % 360 == 180)){
        if (this -> x < width / 3.0) a = 2;
        else if (this -> x < width * 2.0 / 3.0) a = 7;
        else a = 12;
        if (this -> cell.second < 7) b = 1;
        if (this -> cell.second > 7) b = 2;
    }else{
        if (this -> x < width / 3.0) a = 2;
        else if (this -> x < width * 2.0 / 3.0) a = 7;
        else a = 12;
        if ((this -> angle % 360 == 180) or (this -> angle % 360 == 0)){
            if (this -> y < height / 2.0) b = 1;
            else if (this -> y < height * 14.0 / 15.0) b = 2;
            else b = 3;
        }else{
            if (this -> y < height / 2.0) b = 2;
            else b = 3;
        }
    }
    if (this -> state == 5){
        if (this -> cell.second < 7) b = 1;
        if (this -> cell.second > 7) b = 2;
        if ((abs(this -> y - height / 2.0) > height / 30.0) and (this -> angle % 180 == 0)){
            bool ans = (road[a][4] == nullptr);
            ans &= ((road[a][b] == nullptr) or (road[a][b] -> cell == this -> cell));
            if (ans == true){
                road[a][4] = this;
                road[a][b] = this;
            }else this -> priority = 2;
            return ans;
        }
        if (road[a][b] == nullptr) road[a][b] = this;
        if (road[a][b] -> cell != this -> cell) road[a][b] = this;
    }
    if (road[a][b] == nullptr) road[a][b] = this;
    else if (road[a][b] != this) this -> priority = 2;
    return (road[a][b] == this);
}

void Car::releaseroad(int leftroad){
    // leftroad: 留下幾條路的路權
    // state = 5 : 外面的車正在讓裡面的車出來
    if ((this -> state == 5) and (this -> angle % 180 == 90)){
        if (road[2][1] == this) road[2][1] = nullptr;
        if (road[7][1] == this) road[7][1] = nullptr;
        if (road[12][1] == this) road[12][1] = nullptr;
        if (road[2][2] == this) road[2][2] = nullptr;
        if (road[7][2] == this) road[7][2] = nullptr;
        if (road[12][2] == this) road[12][2] = nullptr;
        return;
    }
    int count = 0;
    for (int i = 12 ; i >= 2 ; i -= 5){
        for (int j = 3 ; j >= 1 ; j--){
            if (road[i][j] == this){
                count++;
                if (count > leftroad) road[i][j] = nullptr;
            }
        }
    }
    for (int i = 2 ; i <= 12 ; i += 5){
        if (road[i][0] == this){
            count++;
            if (count > leftroad) road[i][0] = nullptr;
        }
        if (road[i][4] == this){
            count++;
            if (count > leftroad) road[i][4] = nullptr;
        }
    }
    return;
}

bool Car::update() {
    /*
    return false: needs to false this car
    return true: this car still remains
    */
    // part1: update its paraemeter according to its stage
    if (this -> priority == 2) return true; // waiting car in the front.
    if ((this -> state == 0) and (this -> x > width) and (this -> x < width + 0.2)){
        if (this -> cell != std::make_pair(-1,-1)){
            if (getroad() == true) this -> state = 1;
        }else return true;
    }
    if (this -> state == 2){ // stopping at a cell.
        //int probability_inverse = 20000;
        int probability_inverse = 80000;
        if (rand() % probability_inverse == 0){
            this -> state = 3;
            this -> priority = 1;
            this -> brake_timer = 100;
            if (this -> cell.first % 5 < 2) this -> angle = 720;
            if (this -> cell.first % 5 > 2) this -> angle = 900;
        }
        return true;
    }
    if ((this -> state == 3) or (this -> state == 6)){
        getroad();
        return true;
    }
    if ((this -> state == 4) and (this -> x > width) and (this -> x < width + 0.2)) releaseroad(0);
    if (this -> state == 5){
        if ((this -> cell.second <= 10) and (this -> cell.second >= 8) and (this -> angle % 360 == 270) and (abs(this -> y - (this -> cell.second * 2 + 1) * height / 30.0) >= 6) ){
            this -> angle += 180;
        }
        bool cango;
        if (this -> cell.second % 7 >= 4) cango = (abs(this -> y - (this -> cell.second * 2 + 1) * height / 30.0) >= 6);
        if (this -> cell.second % 7 <= 3) cango = ((this -> angle % 360 == 0) and (abs(this -> x - ((this -> cell.first / 5 * 5 + 4) * 2 + 1) * width / 30.0) <= 0.2));
        if (cango == true){
            if (getroad() == true){
                this -> state = 1;
                this -> angle += 180;
            }
        }
    }
    
    // part2: if it is approaching a block (路口)
    std::pair <int,int> approaching = approaching_block(this);
    if (approaching.first == 1) brake_timer = 40; // slow down if near a block.
    if (approaching.first == 2){ // decide direction if near a block.
        if (this -> angle % 360 == 180){
            if (this -> state == 5){
                if ((approaching.second != 0) and (approaching.second != 14)){
                    if (this -> cell.second % 7 <= 3) this -> wheel_timer = 90;
                    if (this -> cell.second % 7 >= 4) this -> wheel_timer = -90;
                }
            }else{
                if (abs(approaching.second - this->cell.first) <= 2){
                    if (abs(this->y - height / 2) <= 0.2){
                        if (this -> cell.second < 7) this -> wheel_timer = -90;
                        else this -> wheel_timer = 90;
                    }
                    else if ((this -> state >= 3) and (this -> cell.second % 7 >= 4)) this -> wheel_timer = -90;
                    else this -> wheel_timer = 90;
                }
                if (getroad() == true) releaseroad(1);
            }
        }else if (this -> angle % 360 == 0){
            if (this -> state == 5){
                if ((approaching.second != 0) and (approaching.second != 14)){
                    if (this -> cell.second % 7 <= 3) this -> wheel_timer = -90;
                    if (this -> cell.second % 7 >= 4) this -> wheel_timer = 90;
                }
            }else{
                if (this -> y < height / 15.0 * 14.0){
                    if (this -> cell.second % 7 >= 4) this -> wheel_timer = 90;
                    else this -> wheel_timer = -90;
                }
                if (getroad() == true) releaseroad(1);
            }
        }else if (this -> angle % 360 == 270){
            if (this -> state == 5){
                if (this -> cell.second <= 3){
                    this -> wheel_timer = 90;
                    releaseroad(1);
                }
            }else{
                if (approaching.second == 14) this -> wheel_timer = 90;
                if (getroad() == true) releaseroad(1);
            }
        }else if (this -> angle % 360 == 90){
            if (this -> state == 5){
                if ((this -> cell.second <= 10) and (this -> cell.second >= 8) ){
                    this -> wheel_timer = -90;
                    releaseroad(1);
                }
            }
        }
    }
    
    // part3 : if it is approaching its cell
    approaching = approaching_cell(this);
    if (approaching.first == 1) this -> brake_timer = 30; // slow down if near a cell.
    if (approaching.first == 2){ // turn right
        this -> wheel_timer = 90;
        this -> priority = 0;
    }
    if (approaching.first == 3){ // turn left
        this -> wheel_timer = -90;
        this -> priority = 0;
    }
    if (approaching.first == 4){ // stop
        releaseroad(0);
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
    if ((this -> state == 4) and (this -> angle % 360 == 90)) this -> angle += 180;
    
    if (this -> priority == 2) return true;
    
    // let the car moves.
    float speed = 0.15;
    if (this->wheel_timer != 0) speed = 0.05;
    else if (this->brake_timer > 0){
        speed = 0.08;
        brake_timer--;
    }
	this->x += speed * cos(angle * pi / 180);
	this->y -= speed * sin(angle * pi / 180);
    
    if (this -> wheel_timer > 0){
        this -> angle++;
        this -> wheel_timer--;
    }
    if (this -> wheel_timer < 0){
        this -> angle--;
        this -> wheel_timer++;
    }
    
    //if ((this->x <= 0) or (this->y <= 0) or (this->y >= height) or (this->x >= width*1.4)) return false;
    if (this -> x >= width * 1.4) return false;
	return true;
}


