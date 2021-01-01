#include "Entity.hpp"

#include <iostream>

Entity::Entity(int hitpoints) :
	nVelocity(),
	nHitpoints(hitpoints),
	nMovementComponent()
{
	initializeMovementComponent();
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


void Entity::updateCurrent(sf::Time dt, CommandQueue&)
{
	// longer time(dt) leads to a bigger step
	
	//move(nVelocity * dt.asSeconds());
	nMovementComponent->update(dt);
}

void Entity::remove()
{
	destroy();
}

void Entity::initializeMovementComponent()
{
	nMovementComponent = std::make_unique<MovementComponent>(*this);
}

MovementComponent* Entity::getMovementComponent() const
{
	return nMovementComponent.get();
}
