#include "Player.h"

/// <summary>
/// Initializes the textures and position of the player. It also sets the uplift of the jetpack to 0.
/// </summary>
void Player::init_player() {
	this->sprite = std::make_unique<sf::Sprite>();
	this->add_texture("Resources/BarryRun1.png");
	this->add_texture("Resources/BarryRun2.png");
	this->add_texture("Resources/BarryRun3.png");
	this->add_texture("Resources/BarryFly.png");
	this->add_texture("Resources/BarryDead.png");
	this->change_sprite_texture(0);
	this->sprite->setPosition(this->player_x_pos, this->player_y_max);
	this->uplift = 0;
}

