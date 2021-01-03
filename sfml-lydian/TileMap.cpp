#include "TileMap.hpp"

#include <iostream>

TileMap::TileMap() :
	SceneNode(Category::None),
	nGridSizeF(32.f),
	nGridSizeU(),
	nNumLayers(1),
	nStartX(0),
	nEndX(0),
	nStartY(0),
	nEndY(0)
{
	nGridSizeU = static_cast<unsigned>(nGridSizeF);

	nMaxSize.x = 60;
	nMaxSize.y = 60;

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

unsigned int TileMap::getGridSize() const
{
	return nGridSizeU;
}

void TileMap::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	//for (auto& x : nPtrMap)
	//{
	//	for (auto& y : x)
	//	{
	//		for (auto& z : y)
	//		{
	//			z->drawCurrent(target, states);
	//		}
	//	}
	//}

	for (int x = nStartX; x < nEndX; x++)
	{
		for (int y = nStartY ; y < nEndY; y++)
		{
			for (auto& z : nPtrMap[x][y])
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
		if (nPtrMap[x][y].size() == 0)		// add if zero
		{
			auto tile = std::make_unique<TileNode>(nGridSizeF);
			tile->setPosition(x * nGridSizeF, y * nGridSizeF);
			nPtrMap[x][y].push_back(std::move(tile));
			std::cout << "Tile added\n";
		}
	}
}

sf::Vector2u TileMap::getMapSize() const
{
	return nMaxSize;
}

void TileMap::setRenderLimit(int startX, int endX, int startY, int endY)
{
	nStartX = startX;
	nEndX = endX;
	nStartY = startY;
	nEndY = endY;
}
