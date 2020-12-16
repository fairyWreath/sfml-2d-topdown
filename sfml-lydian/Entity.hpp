#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <SFML/Graphics.hpp>


// base class for entities: game elements
class Entity
{
public:
	// set velocity
	void setVelocity(sf::Vector2f velocity);			// set velocity using sf velocity type
	void setVelocity(float vx, float vy);						// set velocity from float types
	
	// get the velocity
	sf::Vector2f getVelocity() const;

private:
	sf::Vector2f nVelocity;

};


#endif