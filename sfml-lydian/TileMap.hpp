#ifndef TILEMAP_HPP
#define TILEMAP_HPP

#include "SceneNode.hpp"
#include "TileNode.hpp"
#include "ResourceIdentifiers.hpp"

#include <array>

class TileMap : public SceneNode
{
public:
	typedef std::unique_ptr<TileNode> TilePtr;

public:
	explicit TileMap(TextureHolder& textures);

	unsigned int getGridSize() const;

	void addTile(const unsigned x, const unsigned y, const unsigned z);
	void removeTile(const unsigned x, const unsigned y, const unsigned z);

	sf::Vector2u getMapSize() const;

	// limit current x y indexes
	void setRenderLimit(int startX, int endX, int startY, int endY);

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

	// limit current drawn
	int nStartX;
	int nEndX;
	int nStartY;
	int nEndY;

	TextureHolder& nTextures;

	std::vector<std::vector<std::vector<TilePtr>>> nPtrMap;
	sf::Vector2u nMaxSize;		// max x y size
	unsigned nGridSizeU;
	float nGridSizeF;
	unsigned nNumLayers;
};



#endif