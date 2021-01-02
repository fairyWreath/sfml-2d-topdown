#include "MovementComponent.hpp"
#include "Entity.hpp"
#include "Utility.hpp"

#include <iostream>

MovementComponent::MovementComponent(Entity& entity) :
	nEntity(&entity),
	nIsOnDirectedMovement(false),
	nMovementSpeed(200.f)
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
		break;
	case Up:
		nEntity->accelerate(sf::Vector2f(0.f, -speed));
		break;
	case Left:
		nEntity->accelerate(sf::Vector2f(-speed, 0.f));
		break;
	case Down:
		nEntity->accelerate(sf::Vector2f(0.f, +speed));
		break;
	}

	if (nIsOnDirectedMovement)
		nIsOnDirectedMovement = false;
}


void MovementComponent::moveToLocation(float x, float y)
{
	nDirectedDestination = sf::Vector2f(x, y);

	std::cout << "Destination X: " << nDirectedDestination.x << std::endl;
	std::cout << "Destination y: " << nDirectedDestination.y << std::endl;


	nDirectedDirection = unitVector(nDirectedDestination - nEntity->getWorldPosition());

	if (!nIsOnDirectedMovement)
		nIsOnDirectedMovement = true;
}


void MovementComponent::setMovementSpeed(float speed)
{
	nMovementSpeed = speed;
}

