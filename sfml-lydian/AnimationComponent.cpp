#include "AnimationComponent.hpp"
#include "Utility.hpp"
#include "Entity.hpp"
#include "ResourceHolder.hpp"

#include <iostream>

AnimationComponent::AnimationComponent(MovementComponent& movementComponent) :
	nMovementComponent(&movementComponent),
	nCurrentKey(0),
	nCurrentSubKey(0),
	nIsCurrentSubAnimation(false),
	nSprite(),
	nMainDuration(sf::Time::Zero)
{

}

AnimationComponent::AnimationComponent(MovementComponent& movementComponent, const sf::Texture& texture) :
	nMovementComponent(&movementComponent),
	nCurrentKey(0),
	nCurrentSubKey(0),
	nIsCurrentSubAnimation(false),
	nSprite(),
	nMainDuration(sf::Time::Zero)
{
	// currently just set for Elesa
	nSprite.setTexture(texture);

	initializeAnimations();
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

sf::FloatRect AnimationComponent::getSpriteGlobalBounds() const
{
	return nSprite.getGlobalBounds();
}

sf::FloatRect AnimationComponent::getSpriteLocalBounds() const
{
	return nSprite.getLocalBounds();
}


void AnimationComponent::updateAnimations()
{
	// get from current movement speed if still zero
	if (nMainDuration.asSeconds() < 0.01f)
	{
		float speed = nMovementComponent->getMovementSpeed();
		float stridesPerSecond = speed / 17.5f;
		sf::Time animationTime = sf::seconds(4.f / stridesPerSecond);
		nAnimations[nCurrentKey].setDuration(animationTime);
		nMainDuration = animationTime;
	}

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
	Animation movementAnimation(*nSprite.getTexture(), nSprite);
//	movementAnimation.setFrameSize(sf::Vector2i(85, 90));
	movementAnimation.setFrameSize(sf::Vector2i(68, 72));
//	movementAnimation.setFrameSize(sf::Vector2i(34, 46));
//	movementAnimation.setFrameSize(sf::Vector2i(51, 54));


	movementAnimation.setNumFrames(16);
	movementAnimation.setRepeating(true);

	// temporary
	float nCharacterSpeed = nMovementComponent->getMovementSpeed();
//	float nCharacterSpeed = 200.f;

	// set duration based on character speed
	// 17.5.f per stride
	// 4 strides per animation
	float stridesPerSecond = nCharacterSpeed / 35.f;
	sf::Time animationTime = sf::seconds(4.f / stridesPerSecond);
	movementAnimation.setDuration(animationTime);
	nMainDuration = animationTime;

	movementAnimation.addSubAnimation(1, 4, 1);
	movementAnimation.addSubAnimation(5, 8, 2);
	movementAnimation.addSubAnimation(9, 12, 3);
	movementAnimation.addSubAnimation(13, 16, 4);

	movementAnimation.addStillFrame(1, 1);
	movementAnimation.addStillFrame(2, 5);
	movementAnimation.addStillFrame(3, 9);
	movementAnimation.addStillFrame(4, 13);

	addAnimation(1, movementAnimation);
	setAnimation(1);
}




