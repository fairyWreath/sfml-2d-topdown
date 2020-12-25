#include "SpriteNode.hpp"

#include <SFML/Graphics/RenderTarget.hpp>

SpriteNode::SpriteNode(const sf::Texture& texture) :
	SceneNode(),
	nSprite(texture)			// set sprite to constructor texture
{
}

// overload constructor with additional IntRect parameter
SpriteNode::SpriteNode(const sf::Texture& texture, const sf::IntRect& textureRect) :
	nSprite(texture, textureRect)
{
}


// override scenenode virtual draw
void SpriteNode::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(nSprite, states);			// pass in sprite and states
}