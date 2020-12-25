#include "Projectile.hpp"
#include "DataTables.hpp"
#include "Utility.hpp"
#include "ResourceHolder.hpp"
#include "EmitterNode.hpp"

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include <cmath>

#include <iostream>

// get table data
namespace
{
	const std::vector<ProjectileData> ProjectileTable = initializeProjectileData();
}

Projectile::Projectile(Type type, const TextureHolder& textures) :
	Entity(1),		// pass in 1 hitpoint
	nType(type),
	nTargetDirection(),
	nSprite(textures.get(ProjectileTable[type].texture))		// sprite based on table
{
	centerOrigin(nSprite);

	// add particles for guided attacks
	if (isGuided())
	{
		std::unique_ptr<EmitterNode> cyanTrail = std::make_unique<EmitterNode>(Particle::CyanHeartBeam);
		cyanTrail->setPosition(0.f, getBoundingRect().height / 2.f);	// set at tail
		attachChild(std::move(cyanTrail));		// attach to scene graph
	}
}


// change direction
void Projectile::guideTowards(sf::Vector2f destination)
{
	// only for guided projectiles
	assert(isGuided());
	nTargetDirection = unitVector(destination - getWorldPosition());
}

bool Projectile::isGuided() const
{
	// based on type
	switch (nType)
	{
	case AlliedNormal:
	case AlliedSingle:
	case AlliedSingleBurst:
	case AlliedSingleQuick:
	case EnemyNormal:
		return false;
	case SpecialHeart:
		return true;
	default:
		return false;
	}
}

// virtual functions to make scene graph
void Projectile::updateCurrent(sf::Time dt, CommandQueue& commands)
{
	// for guided projectiles
	if (isGuided())
	{
		const float approachRate = 200.f;			// speed

		// get new velocity
		sf::Vector2f newVelocity = unitVector(approachRate * dt.asSeconds() * nTargetDirection + getVelocity());
		newVelocity *= getMaxSpeed();
		float angle = std::atan2(newVelocity.y, newVelocity.x);

		setRotation(toDegree(angle) + 90.f);
		setVelocity(newVelocity);
	}

	// update velocity
	Entity::updateCurrent(dt, commands);
}

void Projectile::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(nSprite, states);
}

// bouding rect from forld transform and sprite
sf::FloatRect Projectile::getBoundingRect() const
{
	return getWorldTransform().transformRect(nSprite.getGlobalBounds());
}


unsigned int Projectile::getCategory() const
{
	switch (nType)
	{
	case AlliedNormal:
	case AlliedSingle:
	case AlliedSingleBurst:
	case AlliedSingleQuick:
	case SpecialHeart:
		return Category::AlliedProjectile;
	case EnemyNormal:
		return Category::EnemyProjectile;
	}
}


float Projectile::getMaxSpeed() const
{
	// return from table
	return ProjectileTable[nType].speed;
}

int Projectile::getDamage() const
{
	return ProjectileTable[nType].damage;
}

