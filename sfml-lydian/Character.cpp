#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

#include "Character.hpp"
#include "ResourceHolder.hpp"
#include "Category.hpp"			// return category for commands

#include "Utility.hpp"

#include "DataTables.hpp"

#include "Command.hpp"
#include "CommandQueue.hpp"

#include <cmath>

#include <iostream>

namespace
{
	const std::vector<CharacterData> CharacterTable = initializeCharacterData();
}

// map corresponding character type to Textures::ID
static Textures::ID toTextureID(Character::Type type)
{
	switch (type)
	{
	case Character::DarkMagician:
		return Textures::DarkMagician;
	
	case Character::Shinobu:
		return Textures::Shinobu;

	case Character::Izuko:
		return Textures::Izuko;

	case Character::Hitagi:
		return Textures::Hitagi;
	}
	
	// default
	return Textures::DarkMagician;
}

// character class constructor
Character::Character(Type type, const TextureHolder& textures, const FontHolder& fonts) :
	nType(type),
	Entity(CharacterTable[type].hitpoints),			// pass in hp to entity class
	nHealthDisplay(nullptr),
	nTravelledDistance(0.f),
	nDirectionIndex(0),				// initial index at 0
	nSprite(textures.get(toTextureID(type)))		// get sprite from texture id type
{
	/* align to origin/center
	- when first added, sprite is set to corner
	- get 4-rect bounds an move sprite to center
	*/
	sf::FloatRect bounds = nSprite.getLocalBounds();
	nSprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);		// divide by 2 to set to center


	// create ptr for text display
	std::unique_ptr<TextNode> healthDisplay = std::make_unique<TextNode>(fonts, "");
	nHealthDisplay = healthDisplay.get();			// get raw C pointer
	attachChild(std::move(healthDisplay));			// attach to scene graph

	updateTexts();
}

// get maximum character speed
float Character::getMaxSpeed() const
{
	return CharacterTable[nType].speed;
}

// overidden draw function
void Character::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(nSprite, states);			// draw from the character sprite
}


// override getCategory virtual function for command dispatch
unsigned int Character::getCategory() const
{
	switch (nType)
	{
	case (DarkMagician):					// if matches main character
		return Category::PlayerCharacter;
	default:
		return Category::EnemyCharacter;
	}
}


// update for main loop
void Character::updateCurrent(sf::Time dt, CommandQueue& commands)
{
	// std::cout << "update\n";

	// update movement
	updateMovementPattern(dt);

	// apply velocity, call virtual parent class function here
	Entity::updateCurrent(dt, commands);		

	// update text nodes
	updateTexts();
}


// update for movements
void Character::updateMovementPattern(sf::Time dt)
{
	// get directions memaddress from the table
	const std::vector<Direction>& directions = CharacterTable[nType].directions;
	
//	std::cout << "size of directions" << directions.size() << "\n";

	// if not empty, a pattern / movement exists
	if (!directions.empty())
	{
		float distanceToTravel = directions[nDirectionIndex].distance;		// get total distance based on current index
		if (nTravelledDistance > distanceToTravel)		// if distance already covered
		{
			// go to next index
			nDirectionIndex = (nDirectionIndex + 1) % directions.size(); // module here so it goes back to 0 if reaches end
			nTravelledDistance = 0.f;			// reset travelled distance
		}
		
		// math for movement
		float radians = toRadian(directions[nDirectionIndex].angle + 90.f);
		// get movements based on character speed from table
		float vx = getMaxSpeed() * std::cos(radians);
		float vy = getMaxSpeed() * std::sin(radians);
		// change entity velocity
		setVelocity(vx, vy);
		// add to travelled distance
		nTravelledDistance += getMaxSpeed() * dt.asSeconds();
	}
}


// update Textnodes
void Character::updateTexts()
{
	nHealthDisplay->setString(toString(getHitpoints()) + " HP");
	nHealthDisplay->setPosition(0.f, - 50.f);		// -50 y above character

	// set rotation so text is always upright
	nHealthDisplay->setRotation(-getRotation());

//	std::cout << getRotation() << std::endl;
}