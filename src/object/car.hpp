#ifndef _CAR_HPP
#define _CAR_HPP
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include "../utils/log.hpp"
#include "../utils/imageProcess.hpp"
#include <string>
#include <iostream>

extern const int scale;
extern const int width;
extern const int height;
extern const int space_width;
extern const int space_height;
extern const int upper_space;
extern const int left_space;
extern const int word_space;

class Car{
	public:
        ALLEGRO_BITMAP *img;
        float x;
        float y;
        float speed;
        float angle;
        float wheel;
        std::string ID;
		Car(float _x, float _y, std::string ID, std::string path, int w, int h);
    Car(float _x, float _y, std::string ID, ALLEGRO_BITMAP *_img);
    ~Car();
    bool update() ;
    
};

#endif
