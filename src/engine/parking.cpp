#include "parking.hpp"
#include "../utils/log.hpp"
#include "../utils/imageProcess.hpp"
#include "../object/car.hpp"
#include <iostream>
#include <string>
#include <utility>
#include <random>
#include <time.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#define pi 3.1415926

// check collision
/*
bool collision(Object *a, Object *b){
	if(dynamic_cast<Player*> (a)){
		if(b->x >= a->x && b->x <= a->x + 2 && b->y >= a->y && b->y <= a->y + 2)return true; // for the case that a is Player*
	}else if(dynamic_cast<Player*> (b)){
		if(a->x >= b->x && a->x <= b->x + 2 && a->y >= b->y && a->y <= b->y + 2)return true; // for the case that b is Player*
	}else{
		if(a->x+1 >= b->x && a->x < b->x + 1 && a->y+1 >= b->y && a->y < b->y + 1)return true; // for the case that neither a nor b are Player*
	}
	return false;
}
*/

// transfer integer to string 
std::string intToChar(int number){
	std::string s = std::to_string(number);
	return s;
}

Parking::Parking() {
	//srand(15903359);
    //srand(641309883);
    srand(time(NULL));
	// load font resource
	this->font_small = al_load_ttf_font("./fonts/Pattaya/Pattaya-Regular.ttf", 16, 0);
    this->font = al_load_ttf_font("./fonts/Pattaya/Pattaya-Regular.ttf", 24, 0);
    this->font_large = al_load_ttf_font("./fonts/Pattaya/Pattaya-Regular.ttf", 30, 0);
	if ((!this->font_small) or (!this->font) or (!this->font_large))
		LOG::game_abort("failed to load font: pirulen.ttf");
	// load car image
    this->car_img = ImageProcess::load_bitmap_at_size("./image/car.png", scale*4, scale*2);
    if (!this->car_img){
        LOG::game_abort("failed to load car image");
	}
}

std::string generate_ID(){
    std::string ID;
    for (int i=0 ; i<3 ; i++) ID += rand() % 26 + 'A';
    ID += '-';
    for (int i=0 ; i<3 ; i++) ID += rand() % 10 + '0';
    return ID;
}

int temp = 0;


void Parking::initial(void){
    this -> object_list.clear();
    this -> empty_cell.clear();
    for (int i=0 ; i<15 ; i++){
        if ((i != 0) and (i != 7) and (i != 14)) up.push_back(upper_space + i * cellheight);
        if ((i != 2) and (i != 7) and (i != 12)) left.push_back(left_space + (i) * cellwidth);
    }
    for (int i=0 ; i<15 ; i++){
        for (int j=0 ; j<15 ; j++){
            if ((i!=2) and (i!=7) and (i!=12) and (j!=0) and (j!=7) and (j!=14)){
                int temp = rand() % 5;
                if (temp == 0) this->empty_cell.push_front(std::make_pair(i,j));
                else{
                    Car *car;
                    ALLEGRO_BITMAP *tmp = al_clone_bitmap(this->car_img);
                    car = new Car(width/30.0 + i*width/15.0, height/30.0 + j*height/15.0,  generate_ID(), tmp);
                    car -> cell = std::make_pair(i,j);
                    car -> state = 2;
                    car -> priority = 1;
                    this->object_list.push_back(car);
                }
            }
        }
    }
    for (int i=0 ; i<15 ; i++){
        for (int j=0 ; j<4 ; j++) road[i][j] = nullptr;
    }
    for (int i=0 ; i<10 ; i++){
        for (auto cell = this->empty_cell.begin() ; cell != this->empty_cell.end() ; cell++){
            int a = rand() % 2;
            if (a == 0){
                auto temp = cell;
                cell ++;
                empty_cell.erase(temp);
                empty_cell.push_back(*temp);
            }
        }
    }
}

void Parking::log(void){
    LOG::game_log("runtime %d", runtime);
    for(auto obj : this->object_list){
        if (obj->state != 2) LOG::game_log("car %s is going to cell (%2d, %2d), its priority is %d, its state is %d", obj->ID.c_str(), obj->cell.first, obj->cell.second, obj->priority, obj->state);
    }
    for (int i=2 ; i<=12 ; i+=5){
        for (int j=0 ; j<=4 ; j++){
            if (road[i][j] != nullptr) LOG::game_log("road %2d %d is owned by car %s", i, j, road[i][j] -> ID.c_str());
        }
    }
    return;
}

void Parking::draw(void) {
    temp ++;
	// background color
	al_clear_to_color(al_map_rgb(100, 100, 100));

	// draw rectangle
	al_draw_rectangle(left_space, upper_space,
					left_space + space_width, upper_space + space_height,
					al_map_rgb(255, 255, 255), 0);
    
    al_draw_line(left_space + space_width, upper_space, left_space + space_width, upper_space + cellheight, al_map_rgb(255, 0, 0), 0);
    
    al_draw_line(left_space + space_width, upper_space + space_height, left_space + space_width, upper_space + space_height - cellheight, al_map_rgb(255, 0, 0), 0);
    
    // draw parking spaces
    for (unsigned long i=0 ; i<left.size() ; i++){
        for (unsigned long j=0 ; j<up.size() ; j++){
            al_draw_line(left.at(i), up.at(j), left.at(i), up.at(j)+cellheight,al_map_rgb(255, 255, 255), 0);
            al_draw_line(left.at(i)+cellwidth, up.at(j), left.at(i)+cellwidth, up.at(j)+cellheight, al_map_rgb(255, 255, 255), 0);
        }
    }
    for (unsigned long i=0 ; i<up.size() ; i++){
        for (unsigned long j=0 ; j<left.size() ; j++){
            al_draw_line(left.at(j), up.at(i), left.at(j)+cellwidth, up.at(i),al_map_rgb(255, 255, 255), 0);
            al_draw_line(left.at(j), up.at(i)+cellheight, left.at(j)+cellwidth, up.at(i)+cellheight,al_map_rgb(255, 255, 255), 0);
        }
    }

	// Render title text.
	std::string info_message;
	al_draw_text(this->font_large, al_map_rgb(255, 0, 0), left_space+space_width, upper_space-40,
				ALLEGRO_ALIGN_LEFT, "Entrance");
		
	al_draw_text(this->font_large, al_map_rgb(255, 0, 0), left_space+space_width, upper_space+space_height+10,
				ALLEGRO_ALIGN_LEFT, "Exit");

	// Draw comic.
	for(auto obj : this->object_list){
		al_draw_rotated_bitmap(obj->img, scale * 2, scale * 1, left_space + obj->x * scale * 1, upper_space + obj->y * scale * 2, -obj->angle * 3.1415926 / 180, 0);
        al_draw_text(this->font_small, al_map_rgb(255,255,255), left_space + obj->x * scale * 1, upper_space + obj->y * scale * 2 + 10, ALLEGRO_ALIGN_CENTER, obj->ID.c_str());
	}
	// render scene
	al_flip_display();
}

// release resource
void Parking::destroy(void) {
	// al_destroy_bitmap(this->img);
	al_destroy_font(this->font);
	al_destroy_bitmap(this->car_img);
}

Parking::~Parking(){
	// release object resource
    for (auto obj : this->object_list) delete obj;
	this->destroy();
}

// to keep safe distance between cars and cars
int decide_priority(Car *car, std::list <Car*> object_list){
    // 0 if not moving on road (can ignore)
    // 1 if normal
    // 2 if waiting for other cars
    if (car -> priority == 0) return 0;
    if (car -> angle % 360 == 0) return 1;
    if (car -> state > 0) return 1;
    
    for (auto car2=object_list.begin() ; car2!=object_list.end() ; car2++){
        if (((*car2) -> state == 2) or ((*car2) -> state == 3) or ((*car2) -> state == 6) or ((*car2) == car)) continue;
        float x2 = (*car2) -> x;
        float y2 = (*car2) -> y;
        for (int i=9 ; i<10 ; i++){
            float x1 = car->x + i * 0.2 * cos(car->angle*pi/180);
            float y1 = car->y - i * 0.2 * sin(car->angle*pi/180);
            if ((x1-x2) * (x1-x2) + (y1-y2) * (y1-y2) < 20){
                if ((car -> state != 2) and (runtime % 10 == 0)) LOG::game_log("%s blocked by %s", car -> ID.c_str(), (*car2) -> ID.c_str());
                return 2;
            }
        }
    }
    return 1;
}

// If the car is at entrance but parking lot is full then return false;
std::pair<int, int> Parking::allo_destination(Car *car){
    if (car -> state >= 3) return car -> cell;
    if (car -> cell == std::make_pair(-1,-1)){
        if (this->empty_cell.size() == 0) return std::make_pair(-1,-1);
        std::pair<int, int> temp = this->empty_cell.front();
        this->empty_cell.pop_front();
        return temp;
    }
    std::pair <int,int> temp1_cell = car -> cell, temp2_cell = temp1_cell;
    if (temp1_cell.first % 5 == 1) temp2_cell = std::make_pair(temp1_cell.first-1, temp1_cell.second);
    if (temp1_cell.first % 5 == 3) temp2_cell = std::make_pair(temp1_cell.first+1, temp1_cell.second);
    for (auto cell = empty_cell.begin() ; cell != empty_cell.end() ; cell++){
        if (*cell == temp2_cell){
            LOG::game_log("%s pop cell %2d %2d and push empty cell %2d %2d state %d", car -> ID.c_str(), temp2_cell.first, temp2_cell.second, temp1_cell.first, temp1_cell.second, car -> state);
            empty_cell.erase(cell);
            empty_cell.push_back(temp1_cell);
            if (car -> state == 2){
                if (runtime != 1){
                    car -> state = 5;
                    car -> cell = temp2_cell;
                    bool cango = (car -> cell.second % 7 >= 4);
                    if (cango == false) cango = car -> getroad();
                    if (cango == true){
                        if (car -> x <= 1.1 * width) car -> priority = 0;
                        if (temp1_cell.first % 5 <= 1) car -> angle = 720;
                        if (temp1_cell.first % 5 >= 3) car -> angle = 900;
                        if (car -> ID == "XPS-960") LOG::game_log("angle %d %d", temp1_cell.first, car -> angle);
                    }else car -> state = 2;
                    Car *car2 = nullptr;
                    for(auto obj = object_list.begin() ; obj != object_list.end() ; obj++){
                        if (((*obj)->cell == temp2_cell) and ((*obj) != car)) car2 = *obj;
                    }
                    if (cango == true) car2 -> state = 6;
                    if (cango == false) car2 -> state = 3;
                    car2 -> releaseroad(0);
                }else{
                    car -> state = 1;
                    if (car -> x <= 1.1 * width) car -> priority = 0;
                    if (temp1_cell.first % 5 == 1) car -> angle = 900;
                    if (temp1_cell.first % 5 == 3) car -> angle = 720;
                }
            }
            return temp2_cell;
        }
    }
    return temp1_cell;
}

// don't generate a car if there is already many cars queueing.
bool too_much_car(std::list <Car*> object_list){
    for(auto obj = object_list.begin() ; obj != object_list.end() ; obj++){
        if (((*obj) -> x > width * 1.2) and ((*obj) -> y < height / 20.0)) return true;
    }
    return false;
}

void Parking::update(void) {
	// update game run time
	runtime++;

	// create car
    //int probability_inverse = 150;
    int probability_inverse = 800;
    if (too_much_car(object_list)) probability_inverse = 2147483647;
    if (runtime % 10 == 0) log();
	if(rand() % probability_inverse == 0){
		Car *car;
		ALLEGRO_BITMAP *tmp = al_clone_bitmap(this->car_img);
		car = new Car(width * 1.3, height * 0.5 / 15.0, generate_ID(), tmp);
		this->object_list.push_back(car);
	}

	// update all object in the scene
	for(auto obj = this->object_list.begin() ; obj != this->object_list.end() ;){
        (*obj) -> cell = allo_destination(*obj);
        (*obj) -> priority = decide_priority(*obj, object_list);
        if (((*obj) -> state == 3) or ((*obj) -> state == 6)){
            int x = (*obj) -> cell.first / 5 * 5 + 2, y = 1;
            if ((*obj) -> cell.second > 7) y = 2;
            if (road[x][y] == (*obj)){
                LOG::game_log("%s push empty cell %2d %2d", (*obj) -> ID.c_str(), (*obj)->cell.first, (*obj)->cell.second);
                if ((*obj) -> state == 3) this -> empty_cell.push_back((*obj)->cell);
                (*obj) -> state = 4;
            }
        }
        if((*obj)->update()) obj++;
        else{
			delete *obj;
			obj = this->object_list.erase(obj);
		}
	}
}
