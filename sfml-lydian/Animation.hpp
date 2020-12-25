#ifndef ANIMATION_HPP
#define ANIMATION_HPP

// base class for animations
// does not derive from SceneNode, only inherits drawable and transformable
// animations to be applied in scene nodes

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Time.hpp>

class Animation : public sf::Drawable, public sf::Transformable
{
public:
	Animation();
	explicit Animation(const sf::Texture& texture);

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

	sf::FloatRect getLocalBounds() const;			// to centerOrigin
	sf::FloatRect getGlobalBounds() const;

	// logic done here
	void update(sf::Time dt);

private:
	// for window.draw
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	sf::Sprite nSprite;		// sprite for the 'current' animation
	sf::Vector2i nFrameSize;
	std::size_t nNumFrames;
	std::size_t nCurrentFrame;
	sf::Time nDuration;
	sf::Time nElapsedTime;
	bool nRepeat;
};

#endif