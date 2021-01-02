#include "AnimationComponent.hpp"
#include "Utility.hpp"
#include "Entity.hpp"

#include <iostream>

AnimationComponent::AnimationComponent(MovementComponent& movementComponent) :
	nMovementComponent(&movementComponent),
	nCurrentKey(0),
	nCurrentSubKey(0),
	nIsCurrentSubAnimation(false)
{

}

void AnimationComponent::addAnimation(int key, Animation animation)
{
	centerOrigin(animation);
	nAnimations.insert({ key, animation });
}

void AnimationComponent::addAnimationWithoutCenter(int key, Animation animation)
{
	nAnimations.insert({ key, animation });
}

void AnimationComponent::setAnimation(int key)
{
	assert(nAnimations.find(key) != nAnimations.end());

	if (nAnimations[key].isPaused()) nAnimations[key].resume();

	nCurrentKey = key;
	nIsCurrentSubAnimation = false;
}

void AnimationComponent::setSubAnimation(int animationKey, int subAnimationKey)
{
	assert(nAnimations.find(animationKey) != nAnimations.end());

	// if paused resume
	if (nAnimations[animationKey].isPaused()) nAnimations[animationKey].resume();

	// if current is already played return
	if (nCurrentKey == animationKey && nCurrentSubKey == subAnimationKey && nAnimations[animationKey].isPlaying()) return;

	nCurrentKey = animationKey;
	nCurrentSubKey = subAnimationKey;
	nIsCurrentSubAnimation = true;
	nAnimations[nCurrentKey].playSubAnimation(nCurrentSubKey);

//	std::cout << "Set sub animation\n";
}

void AnimationComponent::update(sf::Time dt)
{
	updateAnimations();

	nAnimations[nCurrentKey].update(dt);
}

void AnimationComponent::draw(sf::RenderTarget& target, sf::RenderStates states) 
{
	target.draw(nAnimations[nCurrentKey], states);
}


void AnimationComponent::updateAnimations()
{
	switch (nMovementComponent->getCurrentState())
	{
	// based on external animations
	case MovementComponent::MOVING_RIGHT:
		setSubAnimation(1, 3);
		break;
	case MovementComponent::MOVING_UP:
		setSubAnimation(1, 4);
		break;
	case MovementComponent::MOVING_LEFT:
		setSubAnimation(1, 2);
		break;
	case MovementComponent::MOVING_DOWN:
		setSubAnimation(1, 1);
//		std::cout << "current state: " << nMovementComponent->getCurrentState() << std::endl;
		break;
	case MovementComponent::STATIONARY:
	{
		switch (nMovementComponent->getPreviousState())
		{
		case MovementComponent::MOVING_RIGHT:
			nAnimations[1].setStillFrame(3);
			break;
		case MovementComponent::MOVING_UP:
			nAnimations[1].setStillFrame(4);
			break;
		case MovementComponent::MOVING_LEFT:
			nAnimations[1].setStillFrame(2);
			break;
		case MovementComponent::MOVING_DOWN:
			nAnimations[1].setStillFrame(1);
			break;
		case MovementComponent::STATIONARY:
			nAnimations[1].setStillFrame(1);
			break;
		}
		break;
	}
	default:
		break;
	}
}

void AnimationComponent::initializeAnimations()
{

}


