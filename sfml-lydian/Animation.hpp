#ifndef ANIMATION_HPP
#define ANIMATION_HPP


#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Time.hpp>

#include <map>
#include <memory>

class Animation : public sf::Drawable, public sf::Transformable
{
public:
	Animation();
	explicit Animation(const sf::Texture& texture);
	explicit Animation(const sf::Texture& texture, sf::Sprite& sprite);

	~Animation();

	void setTexture(const sf::Texture& texture);
	const sf::Texture* getTexture() const;		// get texture pointer

	void setFrameSize(sf::Vector2i nFrameSize);
	sf::Vector2i getFrameSize() const;

	void setNumFrames(std::size_t numFrames);
	std::size_t getNumFrames() const;

	void setDuration(sf::Time duration);
	sf::Time getDuration() const;

	void setRepeating(bool flag);
	bool isRepeating() const;

	void restart();
	bool isFinished() const;

	void pause();
	void resume();
	bool isPaused() const;

	sf::FloatRect getLocalBounds() const;			// to centerOrigin
	sf::FloatRect getGlobalBounds() const;

	// logic done here
	void update(sf::Time dt);

	void addSubAnimation(size_t startingFrame, size_t endingFrame, int key);
	void playSubAnimation(int key);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	void addStillFrame(int key, size_t frame);
	void setStillFrame(int key);

	bool isPlaying() const;

private:
	// for window.draw
	struct SubAnimation
	{
		SubAnimation();
		SubAnimation(int key, size_t startingFrame, size_t endingFrame);
			
		int key;
		std::size_t startingFrame;
		std::size_t endingFrame;
	};

private:
	bool nIsPlaying;
	sf::Sprite* nSprite;			// can own can share
	bool nIsSpriteOwned;

	// stationary frames to return to
	std::map<int, size_t> nStillFrames;

	std::map<int, SubAnimation> nSubAnimations;
	int nCurrentSubAnimationKey;
	bool nIsSubAnimationPlaying;

	bool nIsStill;			// still frame status

	bool nIsPaused;

//	sf::Sprite nSprite;		// sprite for the 'current' animation

	

	sf::Vector2i nFrameSize;
	std::size_t nNumFrames;
	std::size_t nCurrentFrame;
	sf::Time nDuration;
	sf::Time nElapsedTime;

	bool nRepeat;
};

#endif