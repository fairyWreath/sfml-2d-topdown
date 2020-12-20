#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <SFML/Graphics.hpp>

#include "SceneNode.hpp"

// base class for entities: game elements
class Entity : public SceneNode			// inhertits from scene node
{
public:

	// add hitpoints
	explicit Entity(int hitpoints);
	
	// functions to modify hitpoints from outside
	void heal(int amount);
	void damage(int amount);
	void destroy();

	// getting hitpoint values / status from outside
	int getHitpoints() const;
	virtual bool isDestroyed() const;

	// set velocity
	void setVelocity(sf::Vector2f velocity);			// set velocity using sf velocity type
	void setVelocity(float vx, float vy);						// set velocity from float types
	
	// get the velocity
	sf::Vector2f getVelocity() const;

	// accelerate, getVelocity() + new velocity parameter
	void accelerate(sf::Vector2f velocity);
	void accelerate(float vx, float vy);

protected:
	// velocity movement is done here, to be accessed by child class
	virtual void updateCurrent(sf::Time dt, CommandQueue& commands);			// override SceneNode update

	// update for children classes

private:
	// store hitpoint
	int nHitpoints;

	sf::Vector2f nVelocity;

};


#endif