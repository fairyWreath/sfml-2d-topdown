#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <SFML/Graphics.hpp>

#include "SceneNode.hpp"
#include "MovementComponent.hpp"
#include "AnimationComponent.hpp"


// base class for entities: game elements
class Entity : public SceneNode			// inhertits from scene node
{
public:
	// add hitpoints
	explicit Entity(int hitpoints);
	
	explicit Entity(int hitpoints, std::unique_ptr<MovementComponent> movement, 
		std::unique_ptr<AnimationComponent> animation);

	// functions to modify hitpoints from outside
	void heal(int amount);
	void damage(int amount);
	void destroy();

	// getting hitpoint values / status from outside
	int getHitpoints() const;

	virtual void remove();
	virtual bool isDestroyed() const;

	// set velocity
	void setVelocity(sf::Vector2f velocity);			// set velocity using sf velocity type
	void setVelocity(float vx, float vy);						// set velocity from float types
	
	// get the velocity
	sf::Vector2f getVelocity() const;

	// accelerate, getVelocity() + new velocity parameter
	void accelerate(sf::Vector2f velocity);
	void accelerate(float vx, float vy);

	MovementComponent* getMovementComponent() const;
	AnimationComponent* getAnimationComponent() const;


	virtual float getCharacterSpeed() const;
	virtual unsigned int getCategory() const;

protected:
	// velocity movement is done here, to be accessed by child class
	virtual void initializeMovementComponent();
	virtual void initializeAnimationComponent();

	virtual void updateCurrent(sf::Time dt, CommandQueue& commands);			// override SceneNode update

	virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;

protected:
	std::unique_ptr<MovementComponent> nMovementComponent;
	std::unique_ptr<AnimationComponent> nAnimationComponent;

private:
	// store hitpoint
	int nHitpoints;

	float nSpeed;
	sf::Vector2f nVelocity;

};


#endif