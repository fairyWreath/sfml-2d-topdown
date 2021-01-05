#include "TileData.hpp"

#include <iostream>

TileSheetData::TileSheetData(Textures::ID id, sf::IntRect rect) :
	textureID(id),
	textureRect(rect)
{
}

TileSheetData::TileSheetData()
{
}


namespace
{
	sf::IntRect rectFromIndex(int x, int y)
	{
		int gridSize = 48;			// get from tile map?
		return sf::IntRect(gridSize * x, gridSize * y, gridSize, gridSize);
	}
}

std::vector<TileSheetData> initializeTileSheetData()
{

	std::vector<TileSheetData> result(Tiles::TileCount);

	result[Tiles::TallGrass2] = TileSheetData(Textures::Outside2, rectFromIndex(7, 1));
	result[Tiles::Concrete1] = TileSheetData(Textures::Outside2, rectFromIndex(2, 34));
	

	return result;
}
