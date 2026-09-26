#include "Zapper.h"

/// <summary>
/// Loads the textures an creates the instance of the entity sprite.
/// </summary>
void Zapper::init_zapper(){
	this->sprite = std::make_unique<sf::Sprite>();
	this->add_texture("Resources/Zapper1.png");
	this->add_texture("Resources/Zapper2.png");
	this->add_texture("Resources/Zapper3.png");
	this->add_texture("Resources/Zapper4.png");
	this->change_sprite_texture(0);
}

