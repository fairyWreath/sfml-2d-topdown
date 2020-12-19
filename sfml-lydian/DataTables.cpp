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

	data[Character::Shinobu].hitpoints = 400;
	data[Character::Shinobu].speed = 100.f;
	data[Character::Shinobu].texture = Textures::Shinobu;

	data[Character::Izuko].hitpoints = 100;
	data[Character::Izuko].speed = 400.f;
	data[Character::Izuko].texture = Textures::Izuko;

	return data;
}

