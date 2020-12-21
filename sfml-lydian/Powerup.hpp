#ifndef POWERUP_HPP
#define POWERUP_HPP

#include "Entity.hpp"
#include "Command.hpp"
#include "ResourceIdentifiers.hpp"

// base class for power pickups

// forward declare character
class Character;

class Powerup : public Entity
{
public:
	enum Type
	{
		HealthRefill,
		SpecialAttackRefill,
		AttackSpread,
		AttackRate,
//		MovementSpeed,
		TypeCount
	};

public:
	Powerup(Type type, const TextureHolder& textures);

	virtual unsigned int getCategory() const;
	virtual sf::FloatRect getBoundingRect() const;


	// applying function
	void apply(Character& character) const;

protected:
	// override scenenode class drawcurrent
	virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	Type nType;
	sf::Sprite nSprite;
};





#endif