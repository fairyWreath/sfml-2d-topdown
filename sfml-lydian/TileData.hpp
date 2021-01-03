#ifndef TILEDATA_HPP
#define TILEDATA_HPP

#include "TileIdentifiers.hpp"
#include "ResourceIdentifiers.hpp"

#include <SFML/Graphics/Rect.hpp>

#include <vector>

// sheet data
struct TileSheetData
{
	TileSheetData(Textures::ID id, sf::IntRect rec);
	TileSheetData();	// vector not full

	Textures::ID textureID;
	sf::IntRect textureRect;
};


std::vector<TileSheetData> initializeTileSheetData();


#endif