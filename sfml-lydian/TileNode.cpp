#include "TileNode.hpp"
#include "Utility.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

#include <iostream>

TileNode::TileNode(float gridSize) 
{
	nGrid.setSize(sf::Vector2f(gridSize, gridSize));
	nGrid.setFillColor(sf::Color::Magenta);
	nGrid.setOutlineColor(sf::Color::Cyan);
	nGrid.setOutlineThickness(2.f);
}

void TileNode::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	// transform here because this drawCurrent is NOT called from SceneNode draw
	states.transform *= getTransform();

	target.draw(nGrid, states);
}


