#include "MovementComponent.hpp"
#include "Entity.hpp"
#include "Utility.hpp"

#include <iostream>

MovementComponent::MovementComponent(Entity& entity) :
	nEntity(&entity),
	nIsOnDirectedMovement(false),
	nMovementSpeed(200.f),
	nCurrentState(STATIONARY)
{
	
}


void MovementComponent::update(sf::Time dt)
{
	if (nIsOnDirectedMovement)
	{
		float movementSpeed = 200.f;		// get from character class?

		sf::Vector2f newVelocity = unitVector(movementSpeed * dt.asSeconds() * nDirectedDirection + nEntity->getVelocity());
		newVelocity *= nMovementSpeed;
		nEntity->setVelocity(newVelocity);

		if (abs(nEntity->getWorldPosition().x - nDirectedDestination.x) <= 1.5f
			&& abs(nEntity->getWorldPosition().y - nDirectedDestination.y) <= 1.5f)
		{
			nIsOnDirectedMovement = false;
		}
	}

	updateState();

	//std::cout << "Current x: " << nEntity->getWorldPosition().x << std::endl;
	//std::cout << "Current y: " << nEntity->getWorldPosition().y << std::endl;

	nEntity->move(nEntity->getVelocity() * dt.asSeconds());
}

void MovementComponent::moveToDirection(float speed, Direction direction)
{
	switch (direction)
	{
	case Right:
		nEntity->accelerate(sf::Vector2f(+speed, 0.f));
		nCurrentState = MOVING_RIGHT;
		break;
	case Up:
		nEntity->accelerate(sf::Vector2f(0.f, -speed));
		nCurrentState = MOVING_UP;
		break;
	case Left:
		nEntity->accelerate(sf::Vector2f(-speed, 0.f));
		nCurrentState = MOVING_LEFT;
		break;
	case Down:
		nEntity->accelerate(sf::Vector2f(0.f, +speed));
		nCurrentState = MOVING_DOWN;
		break;
	}

	if (nIsOnDirectedMovement)
		nIsOnDirectedMovement = false;
}

void MovementComponent::updateState()
{
	sf::Vector2f velocity = nEntity->getVelocity();

	if (velocity.x == 0.f && velocity.y == 0.f)
		nCurrentState = STATIONARY;
}


void MovementComponent::moveToLocation(float x, float y)
{
	nDirectedDestination = sf::Vector2f(x, y);

	std::cout << "Destination X: " << nDirectedDestination.x << std::endl;
	std::cout << "Destination y: " << nDirectedDestination.y << std::endl;


	std::cout << "Entity category: " << nEntity->getCategory() << std::endl;

	nDirectedDirection = unitVector(nDirectedDestination - nEntity->getWorldPosition());

	if (!nIsOnDirectedMovement)
		nIsOnDirectedMovement = true;

	std::cout << "Heading angle: " << getDirectedAngle() << std::endl;
}

float MovementComponent::getDirectedAngle() const
{
	assert(nIsOnDirectedMovement);

	return toDegree(atan2(nDirectedDirection.y * -1.f, nDirectedDirection.x));
}

void MovementComponent::setMovementSpeed(float speed)
{
	nMovementSpeed = speed;
}


sf::Vector2f MovementComponent::getVelocity() const
{
	return nEntity->getVelocity();
}

MovementComponent::State MovementComponent::getCurrentState() const
{
	return nCurrentState;
}


