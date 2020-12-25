#include "Entity.hpp"

#include <iostream>

Entity::Entity(int hitpoints) :
	nVelocity(),
	nHitpoints(hitpoints)
{
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
	//if (nHitpoints <= 0)
	//{
	//	std::cout << "health is less\n";
	//}
	//else
	//{
	//	std::cout << "health is more\n";
	//}

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
	/* transformable move method
	- move(offset) is a shortcut for setPosition(getPosition() + offset)
	*/
	move(nVelocity * dt.asSeconds());
}

void Entity::remove()
{
	destroy();
}
