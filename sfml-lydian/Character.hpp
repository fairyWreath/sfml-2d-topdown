#ifndef CHARACTER_HPP
#define CHARACTER_HPP

// character class that inherits from Entity

#include <SFML/Graphics/Sprite.hpp>


#include "Entity.hpp"
#include "ResourceIdentifiers.hpp"
#include "TextNode.hpp"
#include "CommandQueue.hpp"

class Character : public Entity			// indirectly inherits SceneNode
{
public:
	// enum to store character types
	enum Type
	{
		DarkMagician,
		Shinobu,
		Izuko,
		Hitagi,
		TypeCount
	};

public:
	// explicit constructor
	Character(Type type, const TextureHolder& textures, const FontHolder& fonts);		// pass in texture resource holder

	// get category for command, overrides base SceneNode function
	virtual unsigned int getCategory() const;

	// get maximum character speed, from data table
	float getMaxSpeed() const;

private:
	// override drawCurrent function of SceneNode
	virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;

	// override updateCurrent
	virtual void updateCurrent(sf::Time dt, CommandQueue& commands);

	// update movement based on direction
	void updateMovementPattern(sf::Time dt);


	// update texts
	void updateTexts();

private:
	Type nType;
	sf::Sprite nSprite;			// store sprite type

	// for movements
	float nTravelledDistance;		// distance already travelled in a direction
	std::size_t nDirectionIndex;	// index to know which direction, index for direction vector


	// pointers to display text
	TextNode* nHealthDisplay;

};


#endif