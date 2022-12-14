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
        int angle;
        int brake_timer; // if > 0 then slow down.
        int wheel_timer; // if > 0 then turn left, if < 0 then turn right.
        std::string ID;
        std::pair<int, int> cell; // the cell it wants to park to
        int state;
        /*
        0 if not enter the parking lot
        1 enter the parking lot, but didn't get to its cell
        2 is in parking cell
        3 ready to leave parking cell, but not leaving
        4 leaves parking cell
        5 是前面的車，後面的車要出來，所以要讓
        */
        int priority;
        /*
        0 if not moving on road (can ignore)
        1 if normal
        2 if waiting for other cars
		*/
        Car(float _x, float _y, std::string ID, std::string path, int w, int h);
    Car(float _x, float _y, std::string ID, ALLEGRO_BITMAP *_img);
    ~Car();
    bool update() ;
    bool getroad() ;
    void releaseroad(int leftroad) ;
    
};

extern Car* road[15][5];

#endif
