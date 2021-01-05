#include "TileMap.hpp"
#include "TileIdentifiers.hpp"

#include <iostream>

TileMap::TileMap(TextureHolder& textures) :
	SceneNode(Category::None),
	nTextures(textures),
	nGridSizeF(48.f),
	nGridSizeU(),
	nNumLayers(1),
	nStartX(0),
	nEndX(0),
	nStartY(0),
	nEndY(0)
{
	nGridSizeU = static_cast<unsigned>(nGridSizeF);

	nMaxSize.x = 200;
	nMaxSize.y = 250;			//8000.f

	nPtrMap.resize(nMaxSize.x);

	for (size_t j = 0; j < nMaxSize.x; j++)
	{
		nPtrMap[j].resize(nMaxSize.y);
	}

	/*for (size_t i = 0; i < nMaxSize.x; i++)
	{
		nPtrMap.push_back(std::vector<std::vector<TilePtr>>());
		for (size_t j = 0; j < nMaxSize.y; j++)
		{
			nPtrMap[i].push_back(std::vector<TilePtr>());
		}
	}*/
}

unsigned int TileMap::getGridSize() const
{
	return nGridSizeU;
}

void TileMap::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (int x = nStartX; x < nEndX; x++)
	{
		for (int y = nStartY ; y < nEndY; y++)
		{
			for (auto& z : nPtrMap[x][y])
			{
				if (z != nullptr)
					z->drawCurrent(target, states);
			}
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


void TileMap::addTile(const unsigned x, const unsigned y, const unsigned z, Tiles::ID tileID)
{
	if (x < nMaxSize.x && y < nMaxSize.y && z < nNumLayers)
	{
		if (nPtrMap[x][y].size() == 0)
		{
			auto tile = std::make_unique<TileNode>(tileID, nGridSizeF, nTextures);
			tile->setPosition(x * nGridSizeF, y * nGridSizeF);
			nPtrMap[x][y].push_back(std::move(tile));
		}
	}
}


void TileMap::addTile(const unsigned x, const unsigned y, const unsigned z)
{
	if (x < nMaxSize.x && y < nMaxSize.y && z < nNumLayers)
	{
		if (nPtrMap[x][y].size() == 0)		
		{
			auto tile = std::make_unique<TileNode>(Tiles::Concrete1, nGridSizeF, nTextures);
			tile->setPosition(x * nGridSizeF, y * nGridSizeF);
			nPtrMap[x][y].push_back(std::move(tile));
		}
	}
}

void TileMap::removeTile(const unsigned x, const unsigned y, const unsigned z)
{
	if (x < nMaxSize.x && y < nMaxSize.y && z < nNumLayers)
	{
		if (nPtrMap[x][y].size() > z && nPtrMap[x][y][z] != nullptr)
		{
			//	nPtrMap[x][y][z].reset(nullptr);
			nPtrMap[x][y].pop_back();
		}
	}
}