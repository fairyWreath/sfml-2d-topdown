#ifndef CHARACTER_HPP
#define CHARACTER_HPP

// character class that inherits from Entity

#include <SFML/Graphics/Sprite.hpp>


#include "Entity.hpp"
#include "ResourceIdentifiers.hpp"


class Character : public Entity			// indirectly inherits SceneNode
{
public:
	// enum to store character types
	enum Type
	{
		DarkMagician,
		Shinobu,
		Izuko,
		TypeCount
	};

public:
	// explicit constructor
	Character(Type type, const TextureHolder& textures);		// pass in texture resource holder

	// get category for command, overrides base SceneNode function
	virtual unsigned int getCategory() const;

private:
	// override drawCurrent function of SceneNode
	virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;


private:
	Type nType;
	sf::Sprite nSprite;			// store sprite type


};


#endif