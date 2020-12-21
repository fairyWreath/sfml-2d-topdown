#ifndef PROJECTILE_HPP
#define PROJECTILE_HPP

#include "Entity.hpp"
#include "ResourceIdentifiers.hpp"

#include <SFML/Graphics/Sprite.hpp>

class Projectile : public Entity
{
public:
	enum Type
	{
		AlliedNormal,			// normal heart projectiles
		AlliedSingle,

		AlliedSingleBurst,
		AlliedSingleQuick,

		EnemyNormal,
		SpecialHeart,
		TypeCount
	};

public:
	Projectile(Type type, const TextureHolder& textures);

	// for movement state
	void guideTowards(sf::Vector2f destination);
	bool isGuided() const;


	virtual unsigned int getCategory() const;			// projectile category
	virtual sf::FloatRect getBoundingRect() const;		// get bouds

	// prjectile values, from datatable
	float getMaxSpeed() const;
	int getDamage() const;

private:
	// override updatecurrent and drawcurrent from SceneNode class
	virtual void updateCurrent(sf::Time dt, CommandQueue& commands);
	virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	Type nType;

	// store sprite here
	sf::Sprite nSprite;

	// direction
	sf::Vector2f nTargetDirection;
};




#endif