#ifndef DATATABLES_HPP
#define DATATABLES_HPP

// store character attributes

#include "ResourceIdentifiers.hpp"
#include "Category.hpp"

#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Rect.hpp>

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

	// texture rect
	sf::IntRect textureRect;

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

struct ParticleData
{
	sf::Color color;
	sf::Time lifetime;
};


namespace CollisionPair
{
	enum Type
	{
		// types of collision between entities
		PlayerCharacterEnemyCharacter,
		AlliedProjectileEnemyCharacter,
		EnemyProjectilePlayerCharacter,
		PlayerCharacterPowerup,
		TypeCount,
	};
}

// for collisions
struct CollisionData
{
	typedef std::pair<Category::Type, Category::Type> pair;
	CollisionData();
	CollisionData(Category::Type first, Category::Type second, CollisionPair::Type cType);
	pair collisionPair;
	CollisionPair::Type type;
};

// initialize data and get their vectors
std::vector<CharacterData> initializeCharacterData();
std::vector<ProjectileData> initializeProjectileData();
std::vector<PowerupData> initializePowerupData();
std::vector<CollisionData> initializeCollisionData();
std::vector<ParticleData> initializeParticleData();
#endif