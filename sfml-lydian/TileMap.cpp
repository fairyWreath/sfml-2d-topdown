#include "TileMap.hpp"

#include <iostream>

TileMap::TileMap() :
	SceneNode(Category::None)
{
	nGridSizeF = 80.f;
	nGridSizeU = static_cast<unsigned>(nGridSizeF);
	nNumLayers = 1;

	nMaxSize.x = 16;
	nMaxSize.y = 9;

	for (size_t i = 0; i < nMaxSize.x; i++)
	{
		nPtrMap.push_back(std::vector<std::vector<TilePtr>>());
		for (size_t j = 0; j < nMaxSize.y; j++)
		{
			nPtrMap[i].push_back(std::vector<TilePtr>());
			/*for (size_t k = 0; k < nNumLayers; k++)
			{
				nMap[i][j].push_back(Tile(nGridSizeF, i * nGridSizeF, j * nGridSizeF));
			}*/
		}
	}
}


void TileMap::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (auto& x : nPtrMap)
	{
		for (auto& y : x)
		{
			for (auto& z : y)
			{
				z->drawCurrent(target, states);
			}
		}
	}
}

void TileMap::addTile(const unsigned x, const unsigned y, const unsigned z)
{

	if (x < nMaxSize.x && y < nMaxSize.y && z < nNumLayers)
	{
		if (z <= nPtrMap[x][y].size())
		{
			auto tile = std::make_unique<TileNode>(nGridSizeF);
			tile->setPosition(x * nGridSizeF, y * nGridSizeF);
			nPtrMap[x][y].push_back(std::move(tile));
		}
	}
}