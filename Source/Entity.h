#ifndef ENTITY_H
#define ENTITY_H

#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"
#include "SFML/System.hpp"
#include <memory>

class Entity {
public:
	Entity() {
		textures = std::vector<std::unique_ptr<sf::Texture>>();
	}
	void add_texture(const std::string& file_name);
	void change_sprite_texture(unsigned int index);
	void modify_textures(unsigned int index, const std::string& file_name);
	std::unique_ptr<sf::Sprite> sprite;
protected:
	std::vector<std::unique_ptr<sf::Texture>> textures;
};
#endif