#include "Entity.hpp"


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


void Entity::updateCurrent(sf::Time dt)
{
	// longer time(dt) leads to a bigger step
	/* transformable move method
	- move(offset) is a shortcut for setPosition(getPosition() + offset)
	*/
	move(nVelocity * dt.asSeconds());
}


