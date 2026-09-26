#ifndef GAME_H
#define GAME_H

#include "Player.h"
#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"
#include "SFML/Window.hpp"
#include "Zapper.h"
#include <memory>
#include "Items.h"
#include <vector>

class Item;

class Game {
public:
	int background_width = 1272;
	int background_height = 456;
	int ceiling_height = 50;
	int jetpack_power_ = 15;

	const int entity_height_multiplier = 10;
	const int zapper_cutoff = -50;
	const int refresh_rate_ms_ = 15;
	const int speed_divisor_ = 20;
	const float speedup_constatnt_ = 0.2;
	const int char_size = 35;
	const int text_x_pos = 550;
	const int text_y_pos = 15;
	const int runner_animation_texture_mod_ = 3;
	const int zapper_animation_texture_mod_ = 4;
	const int coin_animation_texture_mod_ = 6;
	const int reverser_animation_texture_mod_ = 2;
	const int zapper_start_x = 1200;
	const int zapper_spacing = 400;
	const int zapper_height_mod = 30;
	const int coin_start_x = 500;
	const int coin_height_mod = 20;
	const int reverser_spacing = 4000;

	Game() : gravity_(8), game_over_(false), speed_(2), coins_(0), gravity_reverser_used_(false) {}
	void run();
	friend class Coin;
	friend class GravityReverser;
private:
	double gravity_;
	bool game_over_;
	double speed_;
	int coins_;
	bool gravity_reverser_used_;
	std::unique_ptr<sf::RenderWindow> window_;
	std::unique_ptr<sf::Texture> background_texture_;
	std::unique_ptr<sf::Sprite> background_;
	std::unique_ptr<sf::Sprite> background2_;
	std::unique_ptr<Player> player_;
	std::vector<std::unique_ptr<Zapper>> zappers_;
	std::vector<std::unique_ptr<Coin>> coin_entities_;
	std::unique_ptr<GravityReverser> gravity_reverser_;
	sf::Font font_;
	sf::Text score_text_;


	void init_game();
	void init_window();
	void init_text();
	void init_background();
	void init_zappers();
	void init_coins();
	void init_reverser();
	void reset_game();
	void reset_zappers();
	void reset_player();
	void reset_coins();
	void reset_reverser();
	void increase_speed();
	void render();
	bool check_collision();
	void animate_entities(double run_idx, double animation_idx);
	void animate_player(int index);
	void animate_zappers(int index);
	void animate_coins(int index);
	void animate_reverser(int index);
	void move_entities();
	void move_player();
	void move_zappers();
	void move_coins();
	void move_reverser();
	void scroll_background();
};
#endif
