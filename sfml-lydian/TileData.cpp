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
	// for 32.f size
	sf::IntRect rectFromIndex(int x, int y)
	{
		int gridSize = 32;
		return sf::IntRect(gridSize * x, gridSize * y, gridSize, gridSize);
	}
}

std::vector<TileSheetData> initializeTileSheetData()
{

	std::vector<TileSheetData> result(Tiles::TileCount);

	result[Tiles::Concrete1] = TileSheetData(Textures::Outside1, rectFromIndex(2, 34));
	

	return result;
}
