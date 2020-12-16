#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

#include "Character.hpp"
#include "ResourceHolder.hpp"

// map corresponding character type to Textures::ID
static Textures::ID toTextureID(Character::Type type)
{
	switch (type)
	{
	case Character::DarkMagician:
		return Textures::DarkMagician;
	}
	
	// default
	return Textures::DarkMagician;
}

// character class constructor
Character::Character(Type type, const TextureHolder& textures) : 
	nType(type),
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