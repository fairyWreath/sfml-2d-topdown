#ifndef TILENODE_HPP
#define TILENODE_HPP

#include "SceneNode.hpp"

#include <SFML/Graphics/RectangleShape.hpp>

class TileNode : public SceneNode
{
public:
	explicit TileNode(float gridSize);

public:
//	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	sf::RectangleShape nGrid;

};



#endif