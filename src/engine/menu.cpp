#include "menu.hpp"
#include "../utils/log.hpp"
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

Menu::Menu(){
    this->font_small = al_load_ttf_font("./fonts/Pattaya/Pattaya-Regular.ttf", 30, 0);
    this->font_large = al_load_ttf_font("./fonts/Pattaya/Pattaya-Regular.ttf", 36, 0);
	if ((!this->font_small) or (!this->font_large))
		LOG::game_abort("failed to load font: pirulen.ttf");
}

void Menu::draw(void){
	// background color
	al_clear_to_color(al_map_rgb(100, 100, 100));

	// draw rectangle
	al_draw_rectangle(left_space, upper_space,
					left_space + space_width, upper_space + space_height,
					al_map_rgb(255, 255, 255), 0);
	// draw info message
	char intro_message[] ="This is the final project of B09902005 and B09901133.";
    char about_message[] ="This project simulates a better parking lot for intelligence vehicles.";
	char press_message[] ="Press any key to play";
	al_draw_text(this->font_large, al_map_rgb(255, 255, 255), SCREEN_W / 2, 250,
				ALLEGRO_ALIGN_CENTER, intro_message);
	al_draw_text(this->font_small, al_map_rgb(255, 255, 255), SCREEN_W / 2, 320,
				ALLEGRO_ALIGN_CENTER, about_message);
    al_draw_text(this->font_large, al_map_rgb(0, 0, 255), SCREEN_W / 2, 400,
                ALLEGRO_ALIGN_CENTER, press_message);
	al_flip_display();
}

void Menu::destroy(void){
	al_destroy_font(this->font_small);
    al_destroy_font(this->font_large);
}

Menu::~Menu(void){
	this->destroy();
}

void Menu::update(void) {
	// check keyboard status
	for(auto i:key_state){
		if(i){
			this->done = true;
		}
	}
}
