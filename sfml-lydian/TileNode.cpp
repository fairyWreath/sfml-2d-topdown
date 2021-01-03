#include "ResourceHolder.hpp"
#include "TileNode.hpp"
#include "Utility.hpp"
#include "TileData.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

#include <iostream>

namespace
{
	const std::vector<TileSheetData> SheetData = initializeTileSheetData();
}

TileNode::TileNode(Tiles::ID ID, float gridSize, TextureHolder& textures) :
	nTextures(textures),
	nID(ID)
{
	nGrid.setSize(sf::Vector2f(gridSize, gridSize));
	nGrid.setFillColor(sf::Color::Green);
	nGrid.setOutlineColor(sf::Color::Cyan);
	nGrid.setOutlineThickness(2.f);


	if (nID != Tiles::Auto)
	{
		nSprite.setTexture(nTextures.get(SheetData[nID].textureID));
		nSprite.setTextureRect(SheetData[nID].textureRect);
	}
}

Tiles::ID TileNode::getTileID() const
{
	return nID;
}

void TileNode::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	// transform here because this drawCurrent is NOT called from SceneNode draw
	states.transform *= getTransform();

	if (nID == Tiles::Auto)
		target.draw(nGrid, states);
	else
		target.draw(nSprite, states);
}


