#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

#include "Character.hpp"
#include "ResourceHolder.hpp"
#include "Category.hpp"			// return category for commands

#include "DataTables.hpp"

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
	}
	
	// default
	return Textures::DarkMagician;
}

// character class constructor
Character::Character(Type type, const TextureHolder& textures) : 
	nType(type),
	Entity(100),			// temporary
	nSprite(textures.get(toTextureID(type)))		// get sprite from texture id type
{
	/* align to origin/center
	- when first added, sprite is set to corner
	- get 4-rect bounds an move sprite to center
	*/
	sf::FloatRect bounds = nSprite.getLocalBounds();
	nSprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);		// divide by 2 to set to center

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