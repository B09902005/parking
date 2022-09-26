#ifndef _MAINGAME_HPP
#define _MAINGAME_HPP

#include <list>
#include "scene.hpp"
#include "../object/car.hpp"

class Parking : public Scene{
	private:
		std::list<Car*> object_list;
	public:
		Parking();
		virtual ~Parking();
		ALLEGRO_FONT *font;
		ALLEGRO_BITMAP *car_img;
		void draw(void) override;
		void destroy(void) override;
		void update(void) override;
		void initial(void);
};

#endif
