#ifndef DATATABLES_HPP
#define DATATABLES_HPP

// store character attributes

#include "ResourceIdentifiers.hpp"

#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Color.hpp>

#include <vector>
#include <functional>

// forward declare character class
class Character;

// utility direction struct
struct Direction
{
	Direction(float angle, float distance) : angle(angle), distance(distance) {}
	float angle;
	float distance;
};


// struct for character attributes
struct CharacterData
{
	int hitpoints;
	float speed;
	Textures::ID texture;
	sf::Time actionInterval;			// inteval to eg., shoot projectiles

	// storing directions
	std::vector<Direction> directions;
};

// data for projectiles
struct ProjectileData
{
	int damage;					// damage / heal
	
	float speed;
	Textures::ID texture;
};


// power pickups
struct PowerupData
{
	// function that acceps character, to modify character, accepts a single Character parameter
	std::function<void(Character&)> action;
	Textures::ID texture;
};

// initialize data and get their vectors
std::vector<CharacterData> initializeCharacterData();
std::vector<ProjectileData> initializeProjectileData();
std::vector<PowerupData> initializePowerupData();

#endif