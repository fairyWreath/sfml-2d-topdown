#ifndef MOVEMENTCOMPONENT_HPP
#define MOVEMENTCOMPONENT_HPP

#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>

class Entity;

class MovementComponent
{
public:
	enum Direction
	{
		Right,
		Up,
		Left,
		Down
	};

public:
	MovementComponent(Entity& entity);

	// move with given player speed
	void moveToDirection(float speed, Direction direction);
	void moveToLocation(float x, float y);

	void update(sf::Time dt);

	void setMovementSpeed(float speed);

private:
	Entity* nEntity;

	bool nIsOnDirectedMovement;
	sf::Vector2f nDirectedDestination;
	sf::Vector2f nDirectedDirection;

	float nMovementSpeed;

};


#endif