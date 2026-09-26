#include "Entity.h"
#include <memory>

/// <summary>
/// Adds a new texture to the vector of textures. It loads the texture from the disk.
/// </summary>
/// <param name="file_name">The filename of the texture we want to add to the entity.</param>
void Entity::add_texture(const std::string& file_name)
{
	auto new_texture = std::make_unique<sf::Texture>();
	new_texture->loadFromFile(file_name);
	this->textures.push_back(std::move(new_texture));
}

/// <summary>
/// Changes the texture of the entity sprite.
/// </summary>
/// <param name="index">An index of the texture in the vector of textures assigned to the entity. If the index is larger than the 
/// total cont of textures then the texture with the index 0 is set.</param>
void Entity::change_sprite_texture(unsigned int index)
{
	if (this->textures.size() > 0 && index > this->textures.size())
	{
		this->sprite->setTexture(*this->textures[0]);
	}
	this->sprite->setTexture(*this->textures[index]);
}

void Entity::modify_textures(unsigned int index, const std::string& file_name) {
	auto new_texture = std::make_unique<sf::Texture>();
	new_texture->loadFromFile(file_name);
	this->textures[index] = std::move(new_texture);
}
