#include "Powerup.hpp"
#include "DataTables.hpp"
#include "Category.hpp"
#include "CommandQueue.hpp"
#include "Utility.hpp"
#include "ResourceHolder.hpp"

#include <SFML/Graphics/RenderTarget.hpp>

namespace
{
	const std::vector<PowerupData> PowerupTable = initializePowerupData();
}

Powerup::Powerup(Type type, const TextureHolder& textures) :
	Entity(1),
	nType(type),
	nSprite(textures.get(PowerupTable[type].texture))
{
	centerOrigin(nSprite);
}

unsigned int Powerup::getCategory() const
{
	return Category::Powerup;
}

void Powerup::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(nSprite);
}

sf::FloatRect Powerup::getBoundingRect() const
{
	// append bounding rects with transformRect
	return getWorldTransform().transformRect(nSprite.getGlobalBounds());
}

void Powerup::apply(Character& character) const
{
	// apply to character, getting the action from the data table
	PowerupTable[nType].action(character);
}







