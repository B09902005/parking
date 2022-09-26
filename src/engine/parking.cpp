#include "mainGame.hpp"
#include "../utils/log.hpp"
#include "../utils/imageProcess.hpp"
#include "../object/object.hpp"
#include "../object/player.hpp"
#include "../object/bullet.hpp"
#include "../object/asteroid.hpp"
#include "../object/potion.hpp"
#include "../AI/model.hpp"
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

// transfer integer to string 
std::string intToChar(int number){
	std::string s = std::to_string(number);
	return s;
}

MainGame::MainGame() {
	srand(time(NULL));
	// load font resource
	this->font = al_load_ttf_font("./fonts/Pattaya/Pattaya-Regular.ttf", 24, 0);
	if (!this->font)
		LOG::game_abort("failed to load font: pirulen.ttf");
	// load object image
    this->asteroid_img = ImageProcess::load_bitmap_at_size("./image/asteroid.png", scale, scale * 2);

    if (!this->asteroid_img){
        LOG::game_abort("failed to load asteroid image");
	}
	// load bullet image
	this->bullet_img = ImageProcess::load_bitmap_at_size("./image/bullet.png", scale, scale * 2);
	if (!this->bullet_img)
		LOG::game_abort("failed to load bullet image");
	// load another bullet image
	this->bullet2_img = ImageProcess::load_bitmap_at_size("./image/bullet2.png", scale, scale * 2);
	if (!this->bullet2_img)
		LOG::game_abort("failed to load bullet2 image");
    // load another bullet image
    this->bullet3_img = ImageProcess::load_bitmap_at_size("./image/bullet3.png", scale, scale * 2);
    if (!this->bullet3_img)
        LOG::game_abort("failed to load bullet3 image");
    // load another bullet image
    this->bullet4_img = ImageProcess::load_bitmap_at_size("./image/bullet4.png", scale, scale * 2);
    if (!this->bullet4_img)
        LOG::game_abort("failed to load bullet4 image");
}

void MainGame::initial(void){
	// clear object list
	this->object_list.clear();
	// reset Player1 and Player2
	this->P1 = new Player(25, 18, 0, 0, "./image/ship1.png", scale * 3, scale * 6);
	this->P2 = new Player(25, 5, 0, 0, "./image/ship2.png", scale * 3, scale * 6);
    this->P3 = new Player(55, 18, 0, 0, "./image/ship3.png", scale * 3, scale * 6);
    this->P4 = new Player(55, 5, 0, 0, "./image/ship4.png", scale * 3, scale * 6);
	
	this->object_list.push_back(this->P1);
	this->object_list.push_back(this->P2);
    this->object_list.push_back(this->P3);
    this->object_list.push_back(this->P4);
}

int temp = 0;
void MainGame::draw(void) {
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
				ALLEGRO_ALIGN_CENTER, "Player 2");
	info_message = "HP:" + intToChar(this->P2->hp);
	al_draw_text(this->font, al_map_rgb(255, 255, 255), left_space + word_space, 15,
				ALLEGRO_ALIGN_CENTER, info_message.c_str());
	info_message = "Bullet_power:" + intToChar(this->P2->bullet_power);
	al_draw_text(this->font, al_map_rgb(255, 255, 255), left_space + word_space * 2, 15,
				ALLEGRO_ALIGN_CENTER, info_message.c_str());
	info_message = "Exp:" + intToChar(this->P2->exp);
	al_draw_text(this->font, al_map_rgb(255, 255, 255), left_space + word_space * 3, 15,
				ALLEGRO_ALIGN_CENTER, info_message.c_str());
	info_message = "Energy:" + intToChar(this->P2->energy);
	al_draw_text(this->font, al_map_rgb(255, 255, 255), left_space + word_space * 4, 15,
				ALLEGRO_ALIGN_CENTER, info_message.c_str());
		
	al_draw_text(this->font, al_map_rgb(255, 255, 255), left_space, 735,
				ALLEGRO_ALIGN_CENTER, "Player 1");
	info_message = "HP:" + intToChar(this->P1->hp);
	al_draw_text(this->font, al_map_rgb(255, 255, 255), left_space + word_space, 735,
				ALLEGRO_ALIGN_CENTER, info_message.c_str());
	info_message = "Bullet_power:" + intToChar(this->P1->bullet_power);
	al_draw_text(this->font, al_map_rgb(255, 255, 255), left_space + word_space * 2, 735,
				ALLEGRO_ALIGN_CENTER, info_message.c_str());
	info_message = "Exp:" + intToChar(this->P1->exp);
	al_draw_text(this->font, al_map_rgb(255, 255, 255), left_space + word_space * 3, 735,
				ALLEGRO_ALIGN_CENTER, info_message.c_str());
	info_message = "Energy:" + intToChar(this->P1->energy);
	al_draw_text(this->font, al_map_rgb(255, 255, 255), left_space + word_space * 4, 735,
				ALLEGRO_ALIGN_CENTER, info_message.c_str());

    al_draw_text(this->font, al_map_rgb(255, 255, 255), left_space, 45,
                ALLEGRO_ALIGN_CENTER, "Player 4");
    info_message = "HP:" + intToChar(this->P4->hp);
    al_draw_text(this->font, al_map_rgb(255, 255, 255), left_space + word_space, 45,
                ALLEGRO_ALIGN_CENTER, info_message.c_str());
    info_message = "Bullet_power:" + intToChar(this->P4->bullet_power);
    al_draw_text(this->font, al_map_rgb(255, 255, 255), left_space + word_space * 2, 45,
                ALLEGRO_ALIGN_CENTER, info_message.c_str());
    info_message = "Exp:" + intToChar(this->P4->exp);
    al_draw_text(this->font, al_map_rgb(255, 255, 255), left_space + word_space * 3, 45,
                ALLEGRO_ALIGN_CENTER, info_message.c_str());
    info_message = "Energy:" + intToChar(this->P4->energy);
    al_draw_text(this->font, al_map_rgb(255, 255, 255), left_space + word_space * 4, 45,
                ALLEGRO_ALIGN_CENTER, info_message.c_str());
    
    al_draw_text(this->font, al_map_rgb(255, 255, 255), left_space, 765,
                ALLEGRO_ALIGN_CENTER, "Player 3");
    info_message = "HP:" + intToChar(this->P3->hp);
    al_draw_text(this->font, al_map_rgb(255, 255, 255), left_space + word_space, 765,
                ALLEGRO_ALIGN_CENTER, info_message.c_str());
    info_message = "Bullet_power:" + intToChar(this->P3->bullet_power);
    al_draw_text(this->font, al_map_rgb(255, 255, 255), left_space + word_space * 2, 765,
                ALLEGRO_ALIGN_CENTER, info_message.c_str());
    info_message = "Exp:" + intToChar(this->P3->exp);
    al_draw_text(this->font, al_map_rgb(255, 255, 255), left_space + word_space * 3, 765,
                ALLEGRO_ALIGN_CENTER, info_message.c_str());
    info_message = "Energy:" + intToChar(this->P3->energy);
    al_draw_text(this->font, al_map_rgb(255, 255, 255), left_space + word_space * 4, 765,
                ALLEGRO_ALIGN_CENTER, info_message.c_str());

	// Draw comic.
	for(auto obj : this->object_list){
		if(dynamic_cast<Player*> (obj)){
			// Player's size is 3 * scale * 3 * (scale * 2) 
			al_draw_bitmap(obj->img, left_space + obj->x * scale, upper_space + obj->y * scale * 2, 0);
		}else{
			// every object's size is scale * (scale * 2)
			al_draw_bitmap(obj->img, left_space + obj->x * scale, upper_space + obj->y * scale * 2, 0);
		}
	}
	// render scene
	al_flip_display();
}

// release resource
void MainGame::destroy(void) {
	// al_destroy_bitmap(this->img);
	al_destroy_font(this->font);
	al_destroy_bitmap(this->asteroid_img);
	al_destroy_bitmap(this->bullet_img);
	al_destroy_bitmap(this->bullet2_img);
}

MainGame::~MainGame(){
	// release object resource
	for(auto obj : this->object_list){
		obj->destroy();
		//delete obj;
	}
	this->destroy();
}

void MainGame::update(void) {
	// set Player speed
	float speed = 1;
	// update game run time
	runtime++;
	// update player parameter
	this->P1->energy = std::min(100, this->P1->energy + 1);
	this->P2->energy = std::min(100, this->P2->energy + 1);
	this->P1->bullet_power += this->P1->exp / 100;
	this->P1->exp %= 100;
	this->P2->bullet_power += this->P2->exp / 100;
	this->P2->exp %= 100;
	this->P1->bullet_cool = std::max(0, this->P1->bullet_cool - 1);
	this->P2->bullet_cool = std::max(0, this->P2->bullet_cool - 1);
    this->P1->potion[3] = std::max(0, this->P1->potion[3] - 1);
    this->P2->potion[3] = std::max(0, this->P2->potion[3] - 1);
    this->P1->potion[4] = std::max(0, this->P1->potion[4] - 1);
    this->P2->potion[4] = std::max(0, this->P2->potion[4] - 1);
    
    this->P3->energy = std::min(100, this->P3->energy + 1);
    this->P4->energy = std::min(100, this->P4->energy + 1);
    this->P3->bullet_power += this->P3->exp / 100;
    this->P3->exp %= 100;
    this->P4->bullet_power += this->P4->exp / 100;
    this->P4->exp %= 100;
    this->P3->bullet_cool = std::max(0, this->P3->bullet_cool - 1);
    this->P4->bullet_cool = std::max(0, this->P4->bullet_cool - 1);
    this->P3->potion[3] = std::max(0, this->P3->potion[3] - 1);
    this->P4->potion[3] = std::max(0, this->P4->potion[3] - 1);
    this->P3->potion[4] = std::max(0, this->P3->potion[4] - 1);
    this->P4->potion[4] = std::max(0, this->P4->potion[4] - 1);

    for(auto it = this->object_list.begin() ; it != this->object_list.end() ; ){
        if(dynamic_cast<Player*> (*it)){
            auto py = dynamic_cast<Player*> (*it);
            if (py -> hp <= 0) it = this->object_list.erase(it);
            else it ++;
        }else it ++;
    }
    
	// if a player dies then the game ends.
	if(this->P1->hp <= 0 && this->P3->hp <= 0){
		this->winner = 2;
		this->done = true;
	}
    
    if(this->P2->hp <= 0 && this->P4->hp <= 0){
        this->winner = 1;
        this->done = true;
    }
    
    int p1x = this->P1->x + this->P1->speedX, p1y = this->P1->y + this->P1->speedY;
    int p2x = this->P2->x + this->P2->speedX, p2y = this->P2->y + this->P2->speedY;
    int p3x = this->P3->x + this->P3->speedX, p3y = this->P3->y + this->P3->speedY;
    int p4x = this->P4->x + this->P4->speedX, p4y = this->P4->y + this->P4->speedY;
    

    int board[ALLEGRO_KEY_MAX];
    Model model;
    // if asdw is pressed then player1 moves.
    // player1 will move from (P1->x, P1->y) to (P1->x + P1->speedX, P1->y + P1->speedY)
if (this->P1->hp > 0){
    /*
	if(key_state[ALLEGRO_KEY_D]){
		this->P1->speedX += speed;
	}
	if(key_state[ALLEGRO_KEY_A]){
		this->P1->speedX -= speed;
	}
	if(key_state[ALLEGRO_KEY_W]){
		this->P1->speedY -= speed;
	}
	if(key_state[ALLEGRO_KEY_S]){
		this->P1->speedY += speed;
	}
     */
    
    model.update(object_list, board, this->P1);
    if(board[ALLEGRO_KEY_D]){
        this->P1->speedX += speed;
    }
    if(board[ALLEGRO_KEY_A]){
        this->P1->speedX -= speed;
    }
    if(board[ALLEGRO_KEY_W]){
        this->P1->speedY -= speed;
    }
    if(board[ALLEGRO_KEY_S]){
        this->P1->speedY += speed;
    }
	
    // We cannot let (P1->x + P1->speedX, P1->y + P1->speedY) be out of bound.
    p1x = this->P1->x + this->P1->speedX, p1y = this->P1->y + this->P1->speedY;
	if(this->P1->x + this->P1->speedX > width - 3 || this->P1->x + this->P1->speedX < 0 ){
		this->P1->speedX = 0;
	}
	if(this->P1->y + this->P1->speedY > height - 3 || this->P1->y + this->P1->speedY < height / 2 ){
		this->P1->speedY = 0;
	}
    if (p1x+2 >= p3x && p1x <= p3x+2 && p1y+2 >= p3y && p1y <= p3y+2 && this->P3->hp > 0){
        this->P1->speedX = 0;
        this->P1->speedY = 0;
    }
    p1x = this->P1->x + this->P1->speedX, p1y = this->P1->y + this->P1->speedY;
    
	// when player1 shoot (press space), create new bullet object in front of player1
    //if(key_state[ALLEGRO_KEY_SPACE] && this->P1->energy >= this->P1->potion[6] && this->P1->bullet_cool == 0){
    if(board[ALLEGRO_KEY_SPACE] && this->P1->energy >= this->P1->potion[6] && this->P1->bullet_cool == 0){
		this->P1->bullet_cool = 5;
		this->P1->energy -= this->P1->potion[6];
		ALLEGRO_BITMAP *tmp = al_clone_bitmap(this->bullet_img);
		Object *bullet = new Bullet(this->P1->x + 1, this->P1->y - 1, 0, -1, tmp, 1);
		this->object_list.push_back(bullet);
	}
}
    
    // if up fght is pressed then player3 moves.
    // player3 will move from (P3->x, P3->y) to (P3->x + P3->speedX, P3->y + P3->speedY)
if (this->P3->hp > 0){
    model.update(object_list, board, this->P3);
    if(board[ALLEGRO_KEY_D]){
        this->P3->speedX += speed;
    }
    if(board[ALLEGRO_KEY_A]){
        this->P3->speedX -= speed;
    }
    if(board[ALLEGRO_KEY_W]){
        this->P3->speedY -= speed;
    }
    if(board[ALLEGRO_KEY_S]){
        this->P3->speedY += speed;
    }
    
    // We cannot let (P3->x + P3->speedX, P3->y + P3->speedY) be out of bound.
    p3x = this->P3->x + this->P3->speedX, p3y = this->P3->y + this->P3->speedY;
    if(this->P3->x + this->P3->speedX > width - 3 || this->P3->x + this->P3->speedX < 0 ){
        this->P3->speedX = 0;
    }
    if(this->P3->y + this->P3->speedY > height - 3 || this->P3->y + this->P3->speedY < height / 2 ){
        this->P3->speedY = 0;
    }
    if (p3x+2 >= p1x && p3x <= p1x+2 && p3y+2 >= p1y && p3y <= p1y+2 && this->P1->hp > 0){
        this->P3->speedX = 0;
        this->P3->speedY = 0;
    }
    p3x = this->P3->x + this->P3->speedX, p3y = this->P3->y + this->P3->speedY;
    
    // when player3 shoot (press space), create new bullet object in front of player3
    if(board[ALLEGRO_KEY_SPACE] && this->P3->energy >= this->P3->potion[6] && this->P3->bullet_cool == 0){
        this->P3->bullet_cool = 5;
        this->P3->energy -= this->P3->potion[6];
        ALLEGRO_BITMAP *tmp = al_clone_bitmap(this->bullet3_img);
        Object *bullet = new Bullet(this->P3->x + 1, this->P3->y - 1, 0, -2, tmp, 3);
        this->object_list.push_back(bullet);
    }
}
    

    model.update_ans(object_list, board, this->P2);
    // if up down left right is pressed then player2 moves.
if (this->P2->hp > 0){
	if(board[ALLEGRO_KEY_D]){
		this->P2->speedX += speed;
	}
	if(board[ALLEGRO_KEY_A]){
		this->P2->speedX -= speed;
	}
	if(board[ALLEGRO_KEY_W]){
		this->P2->speedY -= speed;
	}
	if(board[ALLEGRO_KEY_S]){
		this->P2->speedY += speed;
	}
    
    // We cannot let (P2->x + P2->speedX, P2->y + P2->speedY) be out of bound.
    p2x = this->P2->x + this->P2->speedX, p2y = this->P2->y + this->P2->speedY;
	if(this->P2->x + this->P2->speedX > width - 3 || this->P2->x + this->P2->speedX < 0 ){
		this->P2->speedX = 0;
	}
	if(this->P2->y + this->P2->speedY > height / 2 - 3 || this->P2->y + this->P2->speedY < 0 ){
		this->P2->speedY = 0;
	}
    if (p2x+2 >= p4x && p2x <= p4x+2 && p2y+2 >= p4y && p2y <= p4y+2 && this->P4->hp > 0){
        this->P2->speedX = 0;
        this->P2->speedY = 0;
    }
    p2x = this->P2->x + this->P2->speedX, p2y = this->P2->y + this->P2->speedY;
	
    // when player2 shoot (press enter), create new bullet object in front of player2
	if(board[ALLEGRO_KEY_SPACE] && this->P2->energy >= this->P2->potion[6] && this->P2->bullet_cool == 0){
		this->P2->bullet_cool = 5;
		this->P2->energy -= this->P2->potion[6];
		ALLEGRO_BITMAP *tmp = al_clone_bitmap(this->bullet2_img);
		Object *bullet = new Bullet(this->P2->x + 1, this->P2->y + 3, 0, 1, tmp, 2);
		this->object_list.push_back(bullet);
	}
}
    
    // if jkli is pressed then player4 moves.
    model.update_ans(object_list, board, this->P4);
if (this->P4->hp > 0){
    if(board[ALLEGRO_KEY_D]){
        this->P4->speedX += speed;
    }
    if(board[ALLEGRO_KEY_A]){
        this->P4->speedX -= speed;
    }
    if(board[ALLEGRO_KEY_W]){
        this->P4->speedY -= speed;
    }
    if(board[ALLEGRO_KEY_S]){
        this->P4->speedY += speed;
    }
    
    // We cannot let (P4->x + P4->speedX, P4->y + P4->speedY) be out of bound.
    p4x = this->P4->x + this->P4->speedX, p4y = this->P4->y + this->P4->speedY;
    if(this->P4->x + this->P4->speedX > width - 3 || this->P4->x + this->P4->speedX < 0 ){
        this->P4->speedX = 0;
    }
    if(this->P4->y + this->P4->speedY > height / 2 - 3 || this->P4->y + this->P4->speedY < 0 ){
        this->P4->speedY = 0;
    }
    if (p4x+2 >= p2x && p4x <= p2x+2 && p4y+2 >= p2y && p4y <= p2y+2 && this->P2->hp > 0){
        this->P4->speedX = 0;
        this->P4->speedY = 0;
    }
    p4x = this->P4->x + this->P4->speedX, p4y = this->P4->y + this->P4->speedY;
    
    // when player4 shoot (press enter), create new bullet object in front of player2
    if(board[ALLEGRO_KEY_SPACE] && this->P4->energy >= this->P4->potion[6] && this->P4->bullet_cool == 0){
        this->P4->bullet_cool = 5;
        this->P4->energy -= this->P4->potion[6];
        ALLEGRO_BITMAP *tmp = al_clone_bitmap(this->bullet4_img);
        Object *bullet = new Bullet(this->P4->x + 1, this->P4->y + 3, 0, 1, tmp, 4);
        this->object_list.push_back(bullet);
    }
}

	// create asteroid
	int probability_inverse = 120000 / std::min(120000, runtime);
	if(rand() % probability_inverse == 0){
		int side = rand() % 4;
		Object *asteroid;
		ALLEGRO_BITMAP *tmp = al_clone_bitmap(this->asteroid_img);
		if(side == 0){
			asteroid = new Asteroid(rand() % width, 0, 0, 0.1, tmp);
		}else if(side == 1){
			asteroid = new Asteroid(rand() % width, height, 0, -0.1, tmp);
		}else if(side == 2){
			asteroid = new Asteroid(0, rand() % height, 0.2, 0, tmp);
		}else if(side == 3){
			asteroid = new Asteroid(width, rand() % height, -0.2, 0, tmp);
		}
		this->object_list.push_back(asteroid);
	}

	// create potion
	if(rand() % 600 == 0){
		int type = rand() % 7;
		std::string path = "./image/potion" + intToChar(type) + ".png";
		Object *potion = new Potion(rand() % width, rand() % height, 0, 0, path.c_str(), scale, scale * 2, type);
		this->object_list.push_back(potion);
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

	// check collision and update game
	for(auto from = this->object_list.begin() ; from != this->object_list.end() ;){
		// check erase "from" object
		bool flag = 0;
		for(auto to = this->object_list.begin() ; to != this->object_list.end() ;){
			// not collision or collision to itself
			if(from == to || !collision(*from, *to)){
				to++;
			}else if(dynamic_cast<Player*> (*from) && dynamic_cast<Bullet*> (*to)){
				// player collide to bullet
				auto py = dynamic_cast<Player*> (*from);
				auto bu = dynamic_cast<Bullet*> (*to);
				// decrease player hp by bullet power
                int damage = 0;
				if(bu->type == 1){
                    int temp = rand() % 5;
					if (temp == 0) damage = this->P1->bullet_power * 2;
                    if (temp != 0) damage = this->P1->bullet_power;
                    if (this->P1->potion[4] != 0) damage *= 2;
				}else if(bu->type == 2){
					damage = this->P2->bullet_power;
                    if (this->P2->potion[4] != 0) damage *= 2;
				}else if(bu->type == 3){
                    damage = this->P3->bullet_power;
                    if (this->P3->potion[4] != 0) damage *= 2;
                }else if(bu->type == 4){
                    damage = this->P4->bullet_power;
                    if (this->P4->potion[4] != 0) damage *= 2;
                    py->energy = 0;
                }
                if (py->potion[3] != 0) damage = 0;
                py->hp -= damage;
				// erase bullet
				delete *to;
				to = this->object_list.erase(to);
			}else if(dynamic_cast<Player*> (*from) && dynamic_cast<Asteroid*> (*to)){
				// player collide asteroid
				auto py = dynamic_cast<Player*> (*from);
				// decrease player hp
                if (py->potion[3] == 0) py->hp -= 3;
				// erase asteroid
				delete *to;
				to = this->object_list.erase(to);
			}else if(dynamic_cast<Bullet*> (*from) && dynamic_cast<Asteroid*> (*to)){
				// bullet collide to asteroid
				auto bu = dynamic_cast<Bullet*> (*from);
				// add player experience
				if(bu->type == 1){
					this->P1->exp += this->P1->potion[5];
				}else if(bu->type == 2){
					this->P2->exp += this->P2->potion[5] * 2;
				}else if(bu->type == 3){
                    this->P3->exp += this->P3->potion[5];
                }else if(bu->type == 4){
                    this->P4->exp += this->P4->potion[5];
                }
				// erase from(bullet)
				flag = 1;
				// erase asteroid
				delete *to;
				to = this->object_list.erase(to);
				break;
			}else if(dynamic_cast<Player*> (*from) && dynamic_cast<Potion*> (*to)){
				// player collide to potion
				auto py = dynamic_cast<Player*> (*from);
				auto po = dynamic_cast<Potion*> (*to);
				// check what type of the potion 
				if(po->type == 0){
                    py->hp = std::min(100, py->hp + 30);
				}else if(po->type == 1){
					py->bullet_power += 1;
				}else if(po->type == 2){
					py->energy += 100;
                }else if (po->type == 3){
                    py->potion[3] = 500;
                }else if (po->type == 4){
                    py->potion[4] = 500;
                }else if (po->type == 5){
                    py->potion[5] += 10;
                }else if (po->type == 6){
                    py->potion[6] = std::max(0, py->potion[6] - 5);
                }
				// erase potion
				delete *to;
				to = this->object_list.erase(to);
			}else if(dynamic_cast<Bullet*> (*from) && dynamic_cast<Bullet*> (*to)){
				// bullet collide bullet
                // erase both bullets
				flag = 1;
				delete *to;
				to = this->object_list.erase(to);
				break;
			}else if(dynamic_cast<Asteroid*> (*from) && dynamic_cast<Asteroid*> (*to)){
				// asteroid collide asteroid
                // erase both asteroids
				flag = 1;
				delete *to;
				to = this->object_list.erase(to);
				break;
			}else{
				// other object collide will skip
				to++;
			}
		}
		if(flag){
			delete *from;
			from = this->object_list.erase(from);
		}else{
			from++;
		}
	}
}
