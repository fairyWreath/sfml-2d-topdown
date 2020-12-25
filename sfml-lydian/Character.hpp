#ifndef CHARACTER_HPP
#define CHARACTER_HPP

// character class that inherits from Entity

#include <SFML/Graphics/Sprite.hpp>

#include "Entity.hpp"
#include "ResourceIdentifiers.hpp"
#include "TextNode.hpp"
#include "CommandQueue.hpp"
#include "Projectile.hpp"
#include "Animation.hpp"


class Character : public Entity			// indirectly inherits SceneNode
{
public:
	// enum to store character types
	enum Type
	{
		DarkMagician,
		Shinobu,
		Izuko,
		Hitagi,
		Yotsugi,
		TypeCount
	};

	enum AttackType
	{
		NormalCircular,

		SingleRight,
		SingleUpRight,
		SingleUp,
		SingleUpLeft,
		SingleLeft,
		SingleDownLeft,
		SingleDown,
		SingleDownRight,
	};

public:
	// explicit constructor
	Character(Type type, const TextureHolder& textures, const FontHolder& fonts);		// pass in texture resource holder

	// get category for command, overrides base SceneNode function
	virtual unsigned int getCategory() const;

	virtual sf::FloatRect getBoundingRect() const;
		
	// remove when destroyed, override entitiy class
	// force remove without explosions
	virtual void remove();
	virtual bool isMarkedForRemoval() const;

	bool isAllied() const;

	// get maximum character speed, from data table
	float getMaxSpeed() const;


	// game actions
	void increaseAttackRate();
	void increaseSpread();
	void collectSpecialAttacks(unsigned int count);

	// actual implementation not put here, since we need concept of how much time has elapsed
	// simply marks flags to true
	void launchNormalWithType(AttackType type);
	void launchSpecial();

	// change projectile type
	void changeProjectile();



	// get speed for player class
	float getCharacterSpeed();

	void modifyCharacterSpeed(float amount);

	void boostCharacter();

private:
	// override drawCurrent function of SceneNode
	virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;

	// override updateCurrent
	virtual void updateCurrent(sf::Time dt, CommandQueue& commands);

	// update movement based on direction
	void updateMovementPattern(sf::Time dt);

	// launch current nCurrenAttackType
	void launchNormal();

	void checkPickupDrop(CommandQueue& commands);
	void checkProjectileLaunch(sf::Time dt, CommandQueue& commands);

	
	void createNormalAttack(SceneNode& node, const TextureHolder& textures) const;
	
	// creating a single projectile
	void createProjectile(SceneNode& node, Projectile::Type type, float xOffset, float yOffset, const TextureHolder& textures) const;
	
	// create with direction
	void createProjectile(SceneNode& node, Projectile::Type type, float xOffset, float yOffset, float angle,
		const TextureHolder& textures) const;

	
	void createPowerup(SceneNode& node, const TextureHolder& textures) const;


	// update texts
	void updateTexts();

	// check projectile changes
	void checkProjectileChanges(sf::Time dt);

	// check boost movements
	void checkBoostStatus(sf::Time dt);

private:
	Type nType;
	sf::Sprite nSprite;			// store sprite type

	// actual command code
	Command nLaunchNormalCommand;
	Command nLaunchSpecialCommand;
	

	// animations
	Animation nExplosion;
	bool nShowExplosion;

	// normal attack cooldown
	sf::Time nAttackCountdown;
	sf::Time nCircularAttackCountdown;

	bool nIsMarkedForRemoval;

	// boolean states, to be used in update()
	bool nIsLaunchingNormal;
	bool nIsLaunchingSpecial;

	int nNormalAttackRateLevel;		// fire level
	int nSpreadLevel;
	int nSpecialAmount;			// amount for special attacks

	// boosted movement state
	bool nIsBoosted;
	bool nIsBoostedBefore;
	sf::Time nBoostCountdown;		// boost duration
	sf::Time nBoostCooldown;		// boost cooldown

	// change projectile
	bool nIsProjectileChanged;
	sf::Time nChangeProjectileCountdown;		// projectile countdown

	// for moving characters manually
	float nCharacterSpeed;

	// current attack type
	AttackType nAttackType;

	// current projectile type
	Projectile::Type nCurrentProjectileType;

	// command to drop current powerups from dead enemies
	Command nDropPowerupCommand;

	// for movements
	float nTravelledDistance;		// distance already travelled in a direction
	std::size_t nDirectionIndex;	// index to know which direction, index for direction vector

	// pointers to display text
	TextNode* nHealthDisplay;

	// display amount of special atacks
	TextNode* nSpecialAttackDisplay;
};


#endif