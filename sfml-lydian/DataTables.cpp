#include "DataTables.hpp"
#include "Character.hpp"

// for std::bind() placeholders
using namespace std::placeholders;


std::vector<CharacterData> initializeCharacterData()
{
	// vector based on character amount
	std::vector<CharacterData> data(Character::TypeCount);

	// initialize inidividual character data here

	data[Character::DarkMagician].hitpoints = 200;
	data[Character::DarkMagician].speed = 200.f;
	data[Character::DarkMagician].texture = Textures::DarkMagician;

	// implement zigzag pattern for shinobu
	data[Character::Shinobu].hitpoints = 400;
	data[Character::Shinobu].speed = 100.f;
	data[Character::Shinobu].texture = Textures::Shinobu;
	// create directions here
	data[Character::Shinobu].directions.push_back(Direction(45, 100));		// 45 degrees, 100 units
	data[Character::Shinobu].directions.push_back(Direction(-45, 200));		// -45 degrees, 200 units
	data[Character::Shinobu].directions.push_back(Direction(45, 100));		// 45 degrees, 100 units, back again

	// character and movement pattern for izuko
	data[Character::Izuko].hitpoints = 100;
	data[Character::Izuko].speed = 400.f;
	data[Character::Izuko].texture = Textures::Izuko;
	data[Character::Izuko].directions.push_back(Direction(45, 60));
	data[Character::Izuko].directions.push_back(Direction(180, 30));		// move back 30 units
	data[Character::Izuko].directions.push_back(Direction(-45, 100));	// -45 degrees, 100 units
	data[Character::Izuko].directions.push_back(Direction(-45, 60));
	data[Character::Izuko].directions.push_back(Direction(180, 30));		// move back 30 units
	data[Character::Izuko].directions.push_back(Direction(45, 100));	// 45 degrees, 100 units

	data[Character::Hitagi].hitpoints = 150;
	data[Character::Hitagi].speed = 150.f;



	return data;
}

