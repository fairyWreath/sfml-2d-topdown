#include "Entity.hpp"

#include <memory>
#include <iostream>

Entity::Entity(int hitpoints) :
	nVelocity(),
	nHitpoints(hitpoints),
	nSpeed(200.f)
{
	initializeMovementComponent();
	
//	nAnimationComponent = std::make_unique<AnimationComponent>(*getMovementComponent(), textures.get(CharacterTable[type].texture));
}

Entity::Entity(int hitpoints, std::unique_ptr<MovementComponent> movement,
	std::unique_ptr<AnimationComponent> animation) :
	nVelocity(),
	nHitpoints(hitpoints),
	nSpeed(100.f)
{
	nMovementComponent = std::move(movement);
	nMovementComponent->setEntity(*this);
	nAnimationComponent = std::move(animation);	
}

void Entity::initializeMovementComponent()
{
	nMovementComponent = std::make_unique<MovementComponent>(*this);
}

void Entity::initializeAnimationComponent()
{
	// let character class handle animations
}


// functions for modifying hp
void Entity::damage(int amount)
{
	assert(amount > 0);			// amount has to be positive

	nHitpoints -= amount;
}

void Entity::heal(int amount)
{
	assert(amount > 0);
	nHitpoints += amount;
}

void Entity::destroy()
{
	nHitpoints = 0;
}

bool Entity::isDestroyed() const
{
	return nHitpoints <= 0;
}

int Entity::getHitpoints() const
{
	return nHitpoints;
}

float Entity::getEntitySpeed() const
{
	return nSpeed;
}


// setting the velocity
void Entity::setVelocity(sf::Vector2f velocity)
{
	nVelocity = velocity;
}
void Entity::setVelocity(float vx, float vy)
{
	// set with x and y floats
	nVelocity.x = vx;
	nVelocity.y = vy;
}

sf::Vector2f Entity::getVelocity() const
{
	return nVelocity;
}

// accelerate, getVelocity() + new velocity parameter
void Entity::accelerate(sf::Vector2f velocity)
{
	nVelocity += velocity;
}

void Entity::accelerate(float vx, float vy)
{
	nVelocity.x += vx;
	nVelocity.y += vy;
}


void Entity::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	nAnimationComponent->draw(target, states);
}


void Entity::updateCurrent(sf::Time dt, CommandQueue&)
{
	nAnimationComponent->update(dt);
	nMovementComponent->update(dt);
}

unsigned int Entity::getCategory() const
{
	return Category::PlayerCharacter;
}


void Entity::remove()
{
	destroy();
}

AnimationComponent* Entity::getAnimationComponent() const
{
	return nAnimationComponent.get();
}

MovementComponent* Entity::getMovementComponent() const
{
	return nMovementComponent.get();
}
