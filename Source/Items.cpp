#include "Items.h"
#include "Game.h"

void Coin::affect_game(Game& game)
{
	game.coins_++;
}

void Coin::reset_effect(Game& game){}

void Coin::init_coin(){
	this->sprite = std::make_unique<sf::Sprite>();
	this->add_texture("Resources/Coin1.png");
	this->add_texture("Resources/Coin2.png");
	this->add_texture("Resources/Coin3.png");
	this->add_texture("Resources/Coin4.png");
	this->add_texture("Resources/Coin5.png");
	this->add_texture("Resources/Coin6.png");
	this->change_sprite_texture(0);
}

void GravityReverser::affect_game(Game& game) {
	game.gravity_reverser_used_ = true;
	game.player_->modify_textures(0, "Resources/BarryRun1_flip.png");
	game.player_->modify_textures(1, "Resources/BarryRun2_flip.png");
	game.player_->modify_textures(2, "Resources/BarryRun3_flip.png");
	game.player_->modify_textures(3, "Resources/BarryFly_flip.png");
}

void GravityReverser::reset_effect(Game& game) {
	game.gravity_reverser_used_ = false;
	game.player_->modify_textures(0, "Resources/BarryRun1.png");
	game.player_->modify_textures(1, "Resources/BarryRun2.png");
	game.player_->modify_textures(2, "Resources/BarryRun3.png");
	game.player_->modify_textures(3, "Resources/BarryFly.png");
}

void GravityReverser::init_reverser(){
	
	this->sprite = std::make_unique<sf::Sprite>();
	this->add_texture("Resources/Reverser1.png");
	this->add_texture("Resources/Reverser2.png");
	this->change_sprite_texture(0);
}
