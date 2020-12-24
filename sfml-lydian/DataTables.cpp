#include "DataTables.hpp"
#include "Character.hpp"
#include "Projectile.hpp"
#include "Powerup.hpp"

// for std::bind() placeholders
using namespace std::placeholders;


std::vector<CharacterData> initializeCharacterData()
{
	// vector based on character amount
	std::vector<CharacterData> data(Character::TypeCount);

	// initialize inidividual character data here

	data[Character::DarkMagician].hitpoints = 250;
	data[Character::DarkMagician].speed = 200.f;
	data[Character::DarkMagician].texture = Textures::DarkMagician;
	data[Character::DarkMagician].actionInterval = sf::seconds(1);

	// implement zigzag pattern for shinobu
	data[Character::Shinobu].hitpoints = 400;
	data[Character::Shinobu].speed = 100.f;
	data[Character::Shinobu].texture = Textures::Shinobu;
	// create directions here
	data[Character::Shinobu].directions.push_back(Direction(45, 100));		// 45 degrees, 100 units
	data[Character::Shinobu].directions.push_back(Direction(-45, 200));		// -45 degrees, 200 units
	data[Character::Shinobu].directions.push_back(Direction(45, 100));		// 45 degrees, 100 units, back again
	data[Character::Shinobu].actionInterval = sf::seconds(3);


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
	data[Character::Hitagi].texture = Textures::Hitagi;
	data[Character::Hitagi].directions.push_back(Direction(-45, 100));		// 45 degrees, 100 units
	data[Character::Hitagi].directions.push_back(Direction(-135, 200));		// -45 degrees, 200 units
	data[Character::Hitagi].directions.push_back(Direction(-45, 100));		// 45 degrees, 100 units, back again

	data[Character::Yotsugi].hitpoints = 300.f;
	data[Character::Yotsugi].speed = 100.f;
	data[Character::Yotsugi].texture = Textures::Yotsugi;
	data[Character::Yotsugi].actionInterval = sf::seconds(3);



	return data;
}

std::vector<ProjectileData> initializeProjectileData()
{
	std::vector<ProjectileData> data(Projectile::TypeCount);

	data[Projectile::AlliedNormal].damage = 10;
	data[Projectile::AlliedNormal].speed = 300.f;
	data[Projectile::AlliedNormal].texture = Textures::AlliedNormal;

	data[Projectile::AlliedSingle].damage = 40;
	data[Projectile::AlliedSingle].speed = 500.f;
	data[Projectile::AlliedSingle].texture = Textures::AlliedSingle;

	data[Projectile::AlliedSingleBurst].damage = 100;
	data[Projectile::AlliedSingleBurst].speed = 200.f;
	data[Projectile::AlliedSingleBurst].texture = Textures::AlliedSingleBurst;

	data[Projectile::AlliedSingleQuick].damage = 20;
	data[Projectile::AlliedSingleQuick].speed = 900.f;
	data[Projectile::AlliedSingleQuick].texture = Textures::AlliedSingleQuick;


	data[Projectile::EnemyNormal].damage = 10;
	data[Projectile::EnemyNormal].speed = 300.f;
	data[Projectile::EnemyNormal].texture = Textures::EnemyNormal;

	data[Projectile::SpecialHeart].damage = 200;
	data[Projectile::SpecialHeart].speed = 150.f;
	data[Projectile::SpecialHeart].texture = Textures::SpecialHeart;

	return data;
}

std::vector<PowerupData> initializePowerupData()
{
	std::vector<PowerupData> data(Powerup::TypeCount);
	
	data[Powerup::HealthRefill].texture = Textures::HealthRefill;
	data[Powerup::HealthRefill].action = [](Character& ch) {ch.heal(25);  };

	data[Powerup::SpecialAttackRefill].texture = Textures::SpecialAttackRefill;
	data[Powerup::SpecialAttackRefill].action = std::bind(&Character::collectSpecialAttacks, _1, 3);	// add 3 special attacks

	data[Powerup::AttackRate].texture = Textures::AttackRate;
	data[Powerup::AttackRate].action = std::bind(&Character::increaseAttackRate, _1);	// no parameters
	
	data[Powerup::AttackSpread].texture = Textures::AttackSpread;
	data[Powerup::AttackSpread].action = std::bind(&Character::increaseSpread, _1);

	return data;
}


CollisionData::CollisionData(Category::Type first, Category::Type second, CollisionPair::Type cType)
{
	collisionPair = std::make_pair(first, second);
	type = cType;
}

CollisionData::CollisionData()
{

}

std::vector<CollisionData> initializeCollisionData()
{

	std::vector<CollisionData> data(CollisionPair::TypeCount);

	data[CollisionPair::PlayerCharacterEnemyCharacter] = CollisionData(Category::PlayerCharacter,
		Category::EnemyCharacter, CollisionPair::PlayerCharacterEnemyCharacter);

	data[CollisionPair::AlliedProjectileEnemyCharacter] = CollisionData(Category::AlliedProjectile,
		Category::EnemyCharacter, CollisionPair::AlliedProjectileEnemyCharacter);

	data[CollisionPair::EnemyProjectilePlayerCharacter] = CollisionData(Category::EnemyProjectile,
		Category::PlayerCharacter, CollisionPair::EnemyProjectilePlayerCharacter);

	data[CollisionPair::PlayerCharacterPowerup] = CollisionData(Category::PlayerCharacter,
		Category::Powerup, CollisionPair::PlayerCharacterPowerup);

	return data;
}