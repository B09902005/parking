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
	srand(time(NULL));
	// load font resource
	this->font = al_load_ttf_font("./fonts/Pattaya/Pattaya-Regular.ttf", 24, 0);
	if (!this->font)
		LOG::game_abort("failed to load font: pirulen.ttf");
	// load car image
    this->car_img = ImageProcess::load_bitmap_at_size("./image/car.png", scale, scale * 2);
    if (!this->car_img){
        LOG::game_abort("failed to load car image");
	}
}

void Parking::initial(void){
	this->object_list.clear();
}

int temp = 0;
void Parking::draw(void) {
    temp ++;
	// background color
	al_clear_to_color(al_map_rgb(100, 100, 100));

	// draw rectangle
	al_draw_rectangle(left_space, upper_space,
					left_space + space_width, upper_space + space_height,
					al_map_rgb(255, 255, 255), 0);
	// draw mid line
	al_draw_line(left_space, upper_space + (space_height / 2), left_space + space_width, upper_space + (space_height / 2),
   al_map_rgb(255, 255, 255), 0);

	// Render title text.
	std::string info_message;
	al_draw_text(this->font, al_map_rgb(255, 255, 255), left_space, 15,
				ALLEGRO_ALIGN_CENTER, "hello world1");
	info_message = "HP:" + intToChar(2021);
	al_draw_text(this->font, al_map_rgb(255, 255, 255), left_space + word_space, 15,
				ALLEGRO_ALIGN_CENTER, info_message.c_str());
		
	al_draw_text(this->font, al_map_rgb(255, 255, 255), left_space, 735,
				ALLEGRO_ALIGN_CENTER, "hello world2");
	info_message = "HP:" + intToChar(2022);
	al_draw_text(this->font, al_map_rgb(255, 255, 255), left_space + word_space, 735,
				ALLEGRO_ALIGN_CENTER, info_message.c_str());

	// Draw comic.
	for(auto obj : this->object_list){
		al_draw_bitmap(obj->img, left_space + obj->x * scale, upper_space + obj->y * scale * 2, 0);
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
    for(auto obj : this->object_list){
        //obj->destroy();
        delete obj;
    }
	this->destroy();
}

void Parking::update(void) {
	// update game run time
	runtime++;

	// create car
	int probability_inverse = 120000 / std::min(120000, runtime);
    probability_inverse = 100;
    LOG::game_log("%d", runtime);
	if(rand() % probability_inverse == 0){
		Car *car;
		ALLEGRO_BITMAP *tmp = al_clone_bitmap(this->car_img);
		car = new Car(width, height/2, "ADA-111", tmp);
		this->object_list.push_back(car);
	}

	// update all object in the scene
	for(auto obj = this->object_list.begin() ; obj != this->object_list.end() ;){
		if(!(*obj)->update()){
			delete *obj;
			obj = this->object_list.erase(obj);
		}else{
			 obj++;
		}
	}
}
