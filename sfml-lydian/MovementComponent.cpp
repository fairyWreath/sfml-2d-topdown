#include "MovementComponent.hpp"
#include "Entity.hpp"
#include "Utility.hpp"

#include <iostream>

MovementComponent::MovementComponent(Entity& entity) :
	nEntity(&entity),
	nIsOnDirectedMovement(false),
	nMovementSpeed(),
	nCurrentState(STATIONARY),
	nPreviousState(STATIONARY)
{
	nMovementSpeed = nEntity->getEntitySpeed();
}

MovementComponent::MovementComponent() :
	nIsOnDirectedMovement(false),
	nMovementSpeed(0.f),
	nCurrentState(STATIONARY),
	nPreviousState(STATIONARY),
	nEntity(nullptr)
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

	nEntity->move(nEntity->getVelocity() * dt.asSeconds());
}

void MovementComponent::moveToDirection(float speed, Direction direction)
{
	switch (direction)
	{
	case Right:
		nEntity->accelerate(sf::Vector2f(+speed, 0.f));
		nPreviousState = MOVING_RIGHT;
		break;
	case Up:
		nEntity->accelerate(sf::Vector2f(0.f, -speed));
		nPreviousState = MOVING_UP;
		break;
	case Left:
		nEntity->accelerate(sf::Vector2f(-speed, 0.f));
		nPreviousState = MOVING_LEFT;
		break;
	case Down:
		nEntity->accelerate(sf::Vector2f(0.f, +speed));
		nPreviousState = MOVING_DOWN;
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
	else if (velocity.x > 0.f && velocity.y == 0.f)
		nCurrentState = MOVING_RIGHT;
	else if (velocity.x == 0.f && velocity.y < 0.f)
		nCurrentState = MOVING_UP;
	else if (velocity.x < 0.f && velocity.y == 0.f)
		nCurrentState = MOVING_LEFT;
	else if (velocity.x == 0.f && velocity.y > 0.f)
		nCurrentState = MOVING_DOWN;
	else 
		nCurrentState = MOVING_DIRECTED;

//	std::cout << "Current State: " << nCurrentState << std::endl;
}


void MovementComponent::moveToLocation(float x, float y)
{
	nDirectedDestination = sf::Vector2f(x, y);
	nDirectedDirection = unitVector(nDirectedDestination - nEntity->getWorldPosition());

	if (!nIsOnDirectedMovement)
		nIsOnDirectedMovement = true;

	nPreviousState = MOVING_DIRECTED;
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

float MovementComponent::getMovementSpeed() const
{
	return nMovementSpeed;
}

sf::Vector2f MovementComponent::getVelocity() const
{
	return nEntity->getVelocity();
}

MovementComponent::State MovementComponent::getCurrentState() const
{
	return nCurrentState;
}

MovementComponent::State MovementComponent::getPreviousState() const
{
	return nPreviousState;
}

void MovementComponent::setEntity(Entity& entity)
{
	nEntity = &entity;
	nMovementSpeed = nEntity->getEntitySpeed();
}
