#ifndef ANIMATIONCOMPONENT_HPP
#define ANIMATIONCOMPONENT_HPP


// animation class for component

#include "Animation.hpp"
#include "MovementComponent.hpp"


class AnimationComponent
{
public:
	AnimationComponent(MovementComponent& movementComponent);

	virtual void addAnimation(int key, Animation animation);
	virtual void addAnimationWithoutCenter(int key, Animation animation);

	virtual void setAnimation(int key);
	virtual void setSubAnimation(int animationKey, int subAnimationKey);

	virtual void update(sf::Time dt);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states);

protected:
	virtual void initializeAnimations();

	// automatically update based on state
	virtual void updateAnimations();

	int nCurrentKey;
	int nCurrentSubKey;
	bool nIsCurrentSubAnimation;

	std::map<int, Animation> nAnimations;	
	MovementComponent* nMovementComponent;

};



#endif

