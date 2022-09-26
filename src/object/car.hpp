#ifndef _PLAYER_HPP
#define _PLAYER_HPP

#include "object.hpp"
#include <string>


class Player : public Object{
	public:
		Player(float _x, float _y, float _speedX, float _speedy, std::string path, int w, int h);
		virtual bool update() override;
    
        int hp; // between 0 and 100. if hp becomes 0 then the player loses.
        int bullet_power; // if your bullet hit your opponent, then your opponent's hp will decrease by bullet_power.
        int exp; // between 0 and 100. if your bullet hit an asteroid then exp += 20. if exp == 100 then bullet_power += 1.
        int energy; // between 0 and 100. shooting a bullet will decrease by 20.
		int bullet_cool; // between 0 and 5. you can shoot only if bullet_cool == 0.
        // You can check hackmd and other codes for more information.
    
        int potion[7]; // storing the states about the buff of the potions.
        // potion[3]: between 0 and 500. if (potion[3] != 0) then hp won't decrease.
        // potion[4]: between 0 and 500. if (potion[4] != 0) then bullet_power * 2.
        // potion[5]: more than 20, which is the exp added when shooting an asteroid.
        // potion[6]: between 0 and 20, which is the energy needed to shoot.
};

#endif
