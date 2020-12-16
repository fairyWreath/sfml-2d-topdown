#ifndef SPRITENODE_HPP
#define SPRITENODE_HPP

#include "SceneNode.hpp"

#include <SFML/Graphics/Sprite.hpp>

// scene node type that acts as a simple sf::Sprite that can be plugged to the tree graph
// attach it to the background layer of the scene graph

class SpriteNode : public SceneNode
{
public:
	explicit SpriteNode(const sf::Texture& texture);
	SpriteNode(const sf::Texture& texture, const sf::IntRect& rect);

private:
	// override scene node drawcurrent function
	virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	sf::Sprite nSprite;			// hold the sprite
};



#endif