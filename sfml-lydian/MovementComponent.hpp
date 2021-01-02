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

	enum State
	{
		STATIONARY,
		MOVING_RIGHT,
		MOVING_UP,
		MOVING_LEFT,
		MOVING_DOWN,
		MOVING_DIRECTED			// with angle
	};

public:
	MovementComponent(Entity& entity);

	// move with given player speed
	void moveToDirection(float speed, Direction direction);
	void moveToLocation(float x, float y);

	void update(sf::Time dt);

	void setMovementSpeed(float speed);

	sf::Vector2f getVelocity() const;
	State getCurrentState() const;
	State getPreviousState() const;

	// get angle of directed movement
	float getDirectedAngle() const;

private:
	void updateState();

private:

	Entity* nEntity;
	State nCurrentState;
	State nPreviousState;

	bool nIsOnDirectedMovement;
	sf::Vector2f nDirectedDestination;
	sf::Vector2f nDirectedDirection;

	float nMovementSpeed;

};


#endif