#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <SFML/Graphics.hpp>

#include "SceneNode.hpp"

// base class for entities: game elements
class Entity : public SceneNode			// inhertits from scene node
{
public:
	// set velocity
	void setVelocity(sf::Vector2f velocity);			// set velocity using sf velocity type
	void setVelocity(float vx, float vy);						// set velocity from float types
	
	// get the velocity
	sf::Vector2f getVelocity() const;

private:
	virtual void updateCurrent(sf::Time dt);			// override SceneNode update

private:
	sf::Vector2f nVelocity;

};


#endif