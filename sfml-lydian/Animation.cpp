#include "Animation.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Texture.hpp>

#include <iostream>
#include <cassert>

Animation::Animation() :
	nSprite(),
	nFrameSize(),
	nNumFrames(0),
	nCurrentFrame(0),
	nDuration(sf::Time::Zero),
	nElapsedTime(sf::Time::Zero),
	nRepeat(false),
	nIsSubAnimationPlaying(false)
{
}

Animation::Animation(const sf::Texture& texture) :
	nSprite(texture),
	nFrameSize(),
	nNumFrames(0),
	nCurrentFrame(0),
	nDuration(sf::Time::Zero),
	nElapsedTime(sf::Time::Zero),
	nRepeat(false)
{
}

Animation::SubAnimation::SubAnimation(int key, size_t startingFrame, size_t endingFrame) :
	key(key),
	startingFrame(startingFrame),
	endingFrame(endingFrame)
{
}

Animation::SubAnimation::SubAnimation()
{

}

void Animation::addSubAnimation(size_t startingFrame, size_t endingFrame, int key)
{
	nSubAnimations.insert({key, SubAnimation(key, startingFrame, endingFrame)});
}


void Animation::playSubAnimation(int key)
{
	assert(nSubAnimations.find(key) != nSubAnimations.end());

	nCurrentSubAnimationKey = key;
	nIsSubAnimationPlaying = true;
	
	// set current frame to first subanimation frame
	nCurrentFrame = nSubAnimations[key].startingFrame;
}

// main logic implemented here
void Animation::update(sf::Time dt)
{
	// get time per frame for the whole animation
	sf::Time timePerFrame = nDuration / static_cast<float>(nNumFrames);

	if (nIsSubAnimationPlaying)
	{
		timePerFrame = nDuration / static_cast<float>(nSubAnimations[nCurrentSubAnimationKey].endingFrame
			- nSubAnimations[nCurrentSubAnimationKey].startingFrame - 1);
	}
	
	nElapsedTime += dt;

	// get bounds and rect from texture
	sf::Vector2i textureBounds(nSprite.getTexture()->getSize());		// x and y size of the sprite
	sf::IntRect textureRect = nSprite.getTextureRect();

	if (!nIsSubAnimationPlaying) 
	{
		// if at beginning, start at the top left of the sprite
		if (nCurrentFrame == 0)
			textureRect = sf::IntRect(0, 0, nFrameSize.x, nFrameSize.y);


		// move the rect inside the sprite
		while (nElapsedTime >= timePerFrame && (nCurrentFrame <= nNumFrames || nRepeat))
		{

			// add x
			textureRect.left += textureRect.width;
			// if x not in bounds, go to next 'row'
			if (textureRect.left + textureRect.width > textureBounds.x)
			{
				textureRect.left = 0;
				textureRect.top += textureRect.height;	// go to next y
			}

			nElapsedTime -= timePerFrame;		// reset elapsed time

			// if repeat, if at end of frames, go back to first
			if (nRepeat)
			{
				nCurrentFrame = (nCurrentFrame + 1) % nNumFrames;

				// reset if at first
				if (nCurrentFrame == 0)
					textureRect = sf::IntRect(0, 0, nFrameSize.x, nFrameSize.y);
			}
			else
			{
				// add normally if not repeated
				nCurrentFrame++;
			}
		}
	}
	else if (nIsSubAnimationPlaying)
	{
		if (nCurrentFrame == nSubAnimations[nCurrentSubAnimationKey].startingFrame)
		{
			textureRect = sf::IntRect(0, 0, nFrameSize.x, nFrameSize.y);

			int column = static_cast<int>( (nCurrentFrame - 1) % (textureBounds.x / textureRect.width));
			int row = static_cast<int>((nCurrentFrame - 1) / (textureBounds.x / textureRect.width));

			textureRect.left += textureRect.width * column;
			textureRect.top += textureRect.height * row;
		}

		while (nElapsedTime >= timePerFrame && (nCurrentFrame <= nSubAnimations[nCurrentSubAnimationKey].endingFrame || nRepeat))
		{
			textureRect.left += textureRect.width;
			if (textureRect.left + textureRect.width > textureBounds.x)
			{
				textureRect.left = 0;
				textureRect.top += textureRect.height;
			}

			nElapsedTime -= timePerFrame;		

			if (nRepeat)
			{
				nCurrentFrame = (nCurrentFrame + 1);
				if (nCurrentFrame > nSubAnimations[nCurrentSubAnimationKey].endingFrame)
					nCurrentFrame = nSubAnimations[nCurrentSubAnimationKey].startingFrame;

				if (nCurrentFrame == nSubAnimations[nCurrentSubAnimationKey].startingFrame)
				{
					textureRect = sf::IntRect(0, 0, nFrameSize.x, nFrameSize.y);

					int column = static_cast<int>((nCurrentFrame - 1) % (textureBounds.x / textureRect.width));
					int row = static_cast<int>((nCurrentFrame - 1) / (textureBounds.x / textureRect.width));

					textureRect.left += textureRect.width * column;
					textureRect.top += textureRect.height * row;
				}
			}
			else
			{
				nCurrentFrame++;

			//	if (nCurrentFrame > nSubAnimations[nCurrentSubAnimationKey].endingFrame)
			//		nIsSubAnimationPlaying = false;
			}
		}
	}
//	std::cout << "Out of loop\n";
	// set the rect
	nSprite.setTextureRect(textureRect);
}


void Animation::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(nSprite, states);
}

// local bounds within the texture
sf::FloatRect Animation::getLocalBounds() const
{
	return sf::FloatRect(getOrigin(), static_cast<sf::Vector2f>(getFrameSize()));
}

sf::FloatRect Animation::getGlobalBounds() const
{
	return getTransform().transformRect(getLocalBounds());
}

void Animation::restart()
{
	nCurrentFrame = 0;
	nIsSubAnimationPlaying = false;
}

bool Animation::isFinished() const
{
	// finished if all frames have been traversed
	return nCurrentFrame >= nNumFrames;
}

void Animation::setTexture(const sf::Texture& texture)
{
	nSprite.setTexture(texture);
}

const sf::Texture* Animation::getTexture() const
{
	return nSprite.getTexture();			// sf::Sprite getTexture() method
}

void Animation::setFrameSize(sf::Vector2i frameSize)
{
	nFrameSize = frameSize;
}

sf::Vector2i Animation::getFrameSize() const
{
	return nFrameSize;
}

void Animation::setNumFrames(std::size_t numFrames)
{
	nNumFrames = numFrames;
}

std::size_t Animation::getNumFrames() const
{
	return nNumFrames;
}

void Animation::setDuration(sf::Time duration)
{
	nDuration = duration;
}

sf::Time Animation::getDuration() const
{
	return nDuration;
}

void Animation::setRepeating(bool flag)
{
	nRepeat = flag;
}

bool Animation::isRepeating() const
{
	return nRepeat;
}


