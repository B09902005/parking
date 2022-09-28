#ifndef _MAINGAME_HPP
#define _MAINGAME_HPP

#include <list>
#include "scene.hpp"
#include "../object/car.hpp"

class Parking : public Scene{
	private:
		std::list<Car*> object_list;
        std::list<std::pair<int,int> > empty_cell;
	public:
		Parking();
		virtual ~Parking();
        ALLEGRO_FONT *font_small;
        ALLEGRO_FONT *font;
        ALLEGRO_FONT *font_large;
		ALLEGRO_BITMAP *car_img;
		void draw(void) override;
		void destroy(void) override;
		void update(void) override;
		void initial(void);
        bool allo_destination(Car*);
};

#endif
