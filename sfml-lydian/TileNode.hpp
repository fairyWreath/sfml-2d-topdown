#ifndef TILENODE_HPP
#define TILENODE_HPP

#include "SceneNode.hpp"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include "ResourceIdentifiers.hpp"
#include "TileIdentifiers.hpp"

class TileNode : public SceneNode
{
public:
	explicit TileNode(Tiles::ID ID, float gridSize, TextureHolder& textures);

	Tiles::ID getTileID() const;

public:
	virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;



private:
	sf::RectangleShape nGrid;

	sf::IntRect nRect;
	sf::Sprite nSprite;

	TextureHolder& nTextures;

	Tiles::ID nID;
};



#endif