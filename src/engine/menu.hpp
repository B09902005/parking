#ifndef _MENU_HPP
#define _MENU_HPP



#include "scene.hpp"


class Menu : public Scene{
	public:
		Menu();
		~Menu();
		ALLEGRO_FONT *font_small;
        ALLEGRO_FONT *font_large;
		void draw(void) override;
		void destroy(void) override;
		void update(void) override;
};

#endif
