#include "Animation.hpp"
#include "Utility.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Texture.hpp>

#include <iostream>
#include <cassert>

Animation::Animation() :
	nSprite(new sf::Sprite()),
	nFrameSize(),
	nNumFrames(0),
	nCurrentFrame(0),
	nDuration(sf::Time::Zero),
	nElapsedTime(sf::Time::Zero),
	nRepeat(false),
	nIsSubAnimationPlaying(false),
	nIsPaused(false),
	nIsStill(false),
	nIsPlaying(true),
	nIsSpriteOwned(true)
{
}

Animation::Animation(const sf::Texture& texture) :
	nSprite(new sf::Sprite(texture)),
	nFrameSize(),
	nNumFrames(0),
	nCurrentFrame(0),
	nDuration(sf::Time::Zero),
	nElapsedTime(sf::Time::Zero),
	nRepeat(false),
	nIsSubAnimationPlaying(false),
	nIsPaused(false),
	nIsStill(false),
	nIsPlaying(true),
	nIsSpriteOwned(true)
{
}

Animation::Animation(const sf::Texture& texture, sf::Sprite& sprite) :
	nSprite(&sprite),
	nFrameSize(),
	nNumFrames(0),
	nCurrentFrame(0),
	nDuration(sf::Time::Zero),
	nElapsedTime(sf::Time::Zero),
	nRepeat(false),
	nIsSubAnimationPlaying(false),
	nIsPaused(false),
	nIsStill(false),
	nIsPlaying(true),
	nIsSpriteOwned(false)
{
	nSprite->setTextureRect(sf::IntRect(0, 0, nFrameSize.x, nFrameSize.y));
	centerOrigin(*nSprite);
}

Animation::~Animation()
{
	// crashes when inserted into std::map when new sprite is created
	if (nIsSpriteOwned)
		free(nSprite);
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
	assert(nSubAnimations.find(key) == nSubAnimations.end());

	nSubAnimations.insert({key, SubAnimation(key, startingFrame, endingFrame)});
}


void Animation::playSubAnimation(int key)
{
	assert(nSubAnimations.find(key) != nSubAnimations.end());

	if (nIsStill) nIsStill = false;

	nIsPlaying = true;
	nCurrentSubAnimationKey = key;
	nIsSubAnimationPlaying = true;
	
	// set current frame to first subanimation frame
	nCurrentFrame = nSubAnimations[key].startingFrame;
}

// main logic implemented here
void Animation::update(sf::Time dt)
{
	if (isPaused() || nIsStill) return;

	// get time per frame for the whole animation
	sf::Time timePerFrame = nDuration / static_cast<float>(nNumFrames);

	if (nIsSubAnimationPlaying)
	{
		timePerFrame = nDuration / static_cast<float>(nSubAnimations[nCurrentSubAnimationKey].endingFrame
			- nSubAnimations[nCurrentSubAnimationKey].startingFrame + 1);
	}
	
	nElapsedTime += dt;

//	std::cout << "TIMEPERFRAME: " << timePerFrame.asSeconds() << std::endl;

	// get bounds and rect from texture
	sf::Vector2i textureBounds(nSprite->getTexture()->getSize());		// x and y size of the sprite
	sf::IntRect textureRect = nSprite->getTextureRect();

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
				nCurrentFrame++;
		}

		if (nCurrentFrame > nSubAnimations[nCurrentSubAnimationKey].endingFrame)
			nIsPlaying = false;
	}


	/* return to previous rect, if number of frames goes over the limit */
	if (nCurrentFrame > nNumFrames)
	{
		textureRect = sf::IntRect(0, 0, nFrameSize.x, nFrameSize.y);
		nIsPlaying = false;
	}


//	std::cout << "Current Frame: " << nCurrentFrame << std::endl;
//	std::cout << "Current Texture Rect: " << textureRect.left << "  " << textureRect.top << std::endl;

	nSprite->setTextureRect(textureRect);
}

void Animation::addStillFrame(int key, size_t frame)
{
	assert(nStillFrames.find(key) == nStillFrames.end());

	nStillFrames.insert({ key, frame });
}

void Animation::setStillFrame(int key)
{
	assert(nStillFrames.find(key) != nStillFrames.end());

	if (!nIsStill)	nIsStill = true;
	
	nIsPlaying = false;

	if (nCurrentFrame == nStillFrames[key]) return;

	nCurrentFrame = nStillFrames[key];

	sf::Vector2i textureBounds(nSprite->getTexture()->getSize());
	sf::IntRect textureRect = sf::IntRect(0, 0, nFrameSize.x, nFrameSize.y);

	int column = static_cast<int>((nCurrentFrame - 1) % (textureBounds.x / textureRect.width));
	int row = static_cast<int>((nCurrentFrame - 1) / (textureBounds.x / textureRect.width));

	textureRect.left += textureRect.width * column;
	textureRect.top += textureRect.height * row;
	
	nSprite->setTextureRect(textureRect);
}



void Animation::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(*nSprite, states);
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
	nIsStill = false;
	nIsPaused = false;
	nIsPlaying = true;
}

bool Animation::isFinished() const
{
	// finished if all frames have been traversed
	return nCurrentFrame >= nNumFrames;
}

void Animation::setTexture(const sf::Texture& texture)
{
	nSprite->setTexture(texture);
}

const sf::Texture* Animation::getTexture() const
{
	return nSprite->getTexture();			// sf::Sprite getTexture() method
}

void Animation::setFrameSize(sf::Vector2i frameSize)
{
	nFrameSize = frameSize;

	// initialize to frame 1 at start
	nSprite->setTextureRect(sf::IntRect(0, 0, nFrameSize.x, nFrameSize.y));
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


void Animation::pause()
{
	nIsPaused = true;
}

void Animation::resume()
{
	nIsPaused = false;
}

bool Animation::isPaused() const
{
	return nIsPaused;
}

bool Animation::isPlaying() const
{
	return nIsPlaying;
}