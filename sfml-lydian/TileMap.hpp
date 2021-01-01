#ifndef TILEMAP_HPP
#define TILEMAP_HPP

#include "SceneNode.hpp"
#include "Tile.hpp"
#include "TileNode.hpp"

#include <array>

class TileMap : public SceneNode
{
public:
	typedef std::unique_ptr<TileNode> TilePtr;

public:
	TileMap();

	void addTile(const unsigned x, const unsigned y, const unsigned z);

public:
	enum Layers
	{
		Base,
		Ground,
		Air,
		LayerCount
	};

private:
	virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;

private:
//	std::array<SceneNode*, LayerCount> nMapLayers;

	std::vector<std::vector<std::vector<TilePtr>>> nPtrMap;


	sf::Vector2u nMaxSize;		// max x y size
	unsigned nGridSizeU;
	float nGridSizeF;
	unsigned nNumLayers;
};



#endif