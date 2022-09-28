#ifndef _CELL_HPP
#define _CELL_HPP
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include "../utils/log.hpp"
#include "../utils/imageProcess.hpp"

extern const int scale;
extern const int width;
extern const int height;
extern const int space_width;
extern const int space_height;
extern const int upper_space;
extern const int left_space;
extern const int word_space;

class Cell{
	public:
        int posx;
        int posy;
        Car *car;
		Cell(int _x, int _y);
        ~Cell();
};

#endif
