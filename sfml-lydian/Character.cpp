#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

#include "Character.hpp"
#include "ResourceHolder.hpp"
#include "Category.hpp"			// return category for commands

#include "Utility.hpp"

#include "DataTables.hpp"

#include "Command.hpp"
#include "CommandQueue.hpp"

#include "Powerup.hpp"

#include <cmath>

#include <iostream>

namespace
{
	const std::vector<CharacterData> CharacterTable = initializeCharacterData();
}

// character class constructor
Character::Character(Type type, const TextureHolder& textures, const FontHolder& fonts) :
	nType(type),
	Entity(CharacterTable[type].hitpoints),			// pass in hp to entity class
	nHealthDisplay(nullptr),
	nSpecialAttackDisplay(nullptr),
	nTravelledDistance(0.f),
	nDirectionIndex(0),				// initial index at 0
	nIsLaunchingNormal(false),		// set initial firing states to false
	nIsLaunchingSpecial(false),
	nNormalAttackRateLevel(2),	// attack speed here
	nSpreadLevel(1),
	nSpecialAmount(2),
	nAttackCountdown(sf::Time::Zero),
	nCircularAttackCountdown(sf::Time::Zero),
	nIsMarkedForRemoval(false),
	nAttackType(NormalCircular),
	nCurrentProjectileType(Projectile::AlliedSingle),
	nCharacterSpeed(200.f),			// for player character struct mover
	nIsBoosted(false),
	nIsBoostedBefore(false),
	nBoostCountdown(sf::Time::Zero),
	nSprite(textures.get(CharacterTable[type].texture))		// get sprite from texture id type
{
	/* align to origin/center
	- when first added, sprite is set to corner
	- get 4-rect bounds an move sprite to center
	*/
	sf::FloatRect bounds = nSprite.getLocalBounds();
	nSprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);		// divide by 2 to set to center


	// create ptr for text display
	std::unique_ptr<TextNode> healthDisplay = std::make_unique<TextNode>(fonts, "");
	nHealthDisplay = healthDisplay.get();			// get raw C pointer
	attachChild(std::move(healthDisplay));			// attach to scene graph

	updateTexts();

	// creating attack commands, using lambda expressions
    nLaunchNormalCommand.category = Category::SceneVoidLayer;
	nLaunchNormalCommand.action = [this, &textures](SceneNode& node, sf::Time time)
	{
		// use this(character class) and context textures
		// below function is owned by Character class
		createNormalAttack(node, textures);
	};

	nLaunchSpecialCommand.category = Category::SceneVoidLayer;
	nLaunchSpecialCommand.action = [this, &textures](SceneNode& node, sf::Time time)
	{
		// missiles are only a single projectile
		createProjectile(node, Projectile::SpecialHeart, 0.f, 0.5f, textures);
	};


	// commands to allow enemis to drop powerups
	nDropPowerupCommand.category = Category::SceneVoidLayer;
	nDropPowerupCommand.action = [this, &textures](SceneNode& node, sf::Time)
	{
		createPowerup(node, textures);
	};

	
	// default attack type for bots
	if (!isAllied())
	{
		nAttackType = SingleUp;
		nCurrentProjectileType = Projectile::EnemyNormal;
	}
}

// creating attacks
void Character::createNormalAttack(SceneNode& node, const TextureHolder& textures) const
{
	Projectile::Type type = nCurrentProjectileType;

	switch (nAttackType)
	{
	case NormalCircular:
		type = Projectile::AlliedNormal;
		createProjectile(node, type, 0.5f, 0.0f, 0, textures);
		createProjectile(node, type, +0.35f, -0.35f, 45, textures);
		createProjectile(node, type, 0.0f, -0.5f, 90, textures);
		createProjectile(node, type, -0.35f, -0.35f, 135, textures);
		createProjectile(node, type, -0.5f, 0.0f, 180, textures);
		createProjectile(node, type, -0.35f, 0.35f, 225, textures);
		createProjectile(node, type, 0.0f, 0.5f, 270, textures);
		createProjectile(node, type, +0.35f, 0.35f, 315, textures);
		break;
	case SingleRight:
		createProjectile(node, type, 0.5f, 0.0f, 0, textures);
		break;
	case SingleUpRight:
		createProjectile(node, type, +0.35f, -0.35f, 45, textures);
		break;
	case SingleUp:
		createProjectile(node, type, 0.0f, -0.5f, 90, textures);
		break;
	case SingleUpLeft:
		createProjectile(node, type, -0.35f, -0.35f, 135, textures);
		break;
	case SingleLeft:
		createProjectile(node, type, -0.5f, 0.0f, 180, textures);
		break;
	case SingleDownLeft:
		createProjectile(node, type, -0.35f, 0.35f, 225, textures);
		break;
	case SingleDown:
		createProjectile(node, type, 0.0f, 0.5f, 270, textures);
		break;
	case SingleDownRight:
		createProjectile(node, type, +0.35f, 0.35f, 315, textures);
		break;
	}
}


// creating a single projectile
void Character::createProjectile(SceneNode& node, Projectile::Type type, float xOffset, 
	float yOffset, const TextureHolder& textures) const
{

	// create projectile ptr
	std::unique_ptr<Projectile> projectile = std::make_unique<Projectile>(type, textures);

//	std::cout << "created projectile\n";

	// set offset and speed 
	sf::Vector2f offset(xOffset * nSprite.getGlobalBounds().width, yOffset * nSprite.getGlobalBounds().height);
	sf::Vector2f velocity(0, projectile->getMaxSpeed());

	// if allied or enemy, to set direction
	float sign = isAllied() ? -1.f : +1.f;
	projectile->setPosition(getWorldPosition() + offset * sign);
	projectile->setVelocity(velocity * sign);
	
	// attach to scene graph
	node.attachChild(std::move(projectile));
}


void Character::createProjectile(SceneNode& node, Projectile::Type type, float xOffset, float yOffset, float angleDeg,
	const TextureHolder& textures) const
{
	std::unique_ptr<Projectile> projectile = std::make_unique<Projectile>(type, textures);
	sf::Vector2f offset(xOffset * nSprite.getGlobalBounds().width, yOffset * nSprite.getGlobalBounds().height);

	float radians = -toRadian(angleDeg);
	//std::cout << radians << std::endl;
	float vx = projectile->getMaxSpeed() * std::cos(radians);
	float vy = projectile->getMaxSpeed() * std::sin(radians);

	sf::Vector2f velocity(vx, vy);
	projectile->setPosition(getWorldPosition() + offset);
	projectile->setVelocity(velocity);
	
	//std::cout << "created projectile\n";

	/* sf::Transformable.setRotation()
	-> set rotation in angles, clockwise from 12 o clock
	*/
	projectile->setRotation(90 - angleDeg);

	node.attachChild(std::move(projectile));
}


// create powerups
void Character::createPowerup(SceneNode& node, const TextureHolder& textures) const
{
	// get random pickup type
	auto type = static_cast<Powerup::Type>(randomInt(Powerup::TypeCount - 1));		// currently exclude last one

	// create pickup here
	auto powerup = std::make_unique<Powerup>(type, textures);
	powerup->setPosition(getWorldPosition());		// set from current position
	powerup->setVelocity(0.f, 1.f);				// set slow movement
	node.attachChild(std::move(powerup));		// attach to child to draw
}


// get maximum character speed
float Character::getMaxSpeed() const
{
	return CharacterTable[nType].speed;
}

// overidden draw function
void Character::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(nSprite, states);			// draw from the character sprite
}


// override getCategory virtual function for command dispatch
unsigned int Character::getCategory() const
{
	switch (nType)
	{
	case (DarkMagician):					// if matches main character
		return Category::PlayerCharacter;
	default:
		return Category::EnemyCharacter;
	}
}


// update for main loop
void Character::updateCurrent(sf::Time dt, CommandQueue& commands)
{
	// if destroyed, possible drop pickup
	if (isDestroyed())
	{
		checkPickupDrop(commands);

		nIsMarkedForRemoval = true;
		return;
	}

	// check if attacks are fired
	checkProjectileLaunch(dt, commands);

	// update movement
	updateMovementPattern(dt);

	// checkBoostStatus();
	checkBoostStatus(dt);

	// apply velocity, call virtual parent class function here
	Entity::updateCurrent(dt, commands);		

	// update text nodes
	updateTexts();

}

void Character::boostCharacter()
{
	nIsBoosted = true;
}

void Character::checkBoostStatus(sf::Time dt)
{
	if (nIsBoosted && !nIsBoostedBefore && nBoostCountdown <= sf::Time::Zero)
	{
		nCharacterSpeed *= 2;
		nBoostCountdown = sf::seconds(3.f);
		nIsBoostedBefore = true;
		nIsBoosted = false;
	}
	else if (nBoostCountdown <= sf::Time::Zero && nIsBoostedBefore)
	{
		nCharacterSpeed /= 2;
		nIsBoostedBefore = false;
	}
	else if (nBoostCountdown > sf::Time::Zero)
	{
		nBoostCountdown -= dt;
	}
}


// update for movements
void Character::updateMovementPattern(sf::Time dt)
{
	// get directions memaddress from the table
	const std::vector<Direction>& directions = CharacterTable[nType].directions;
	
//	std::cout << "size of directions" << directions.size() << "\n";

	// if not empty, a pattern / movement exists
	if (!directions.empty())
	{
		float distanceToTravel = directions[nDirectionIndex].distance;		// get total distance based on current index
		if (nTravelledDistance > distanceToTravel)		// if distance already covered
		{
			// go to next index
			nDirectionIndex = (nDirectionIndex + 1) % directions.size(); // module here so it goes back to 0 if reaches end
			nTravelledDistance = 0.f;			// reset travelled distance
		}
		
		// math for movement, - radian because positive y -> down
		float radians = -toRadian(directions[nDirectionIndex].angle);
		// get movements based on character speed from table
		float vx = getMaxSpeed() * std::cos(radians);
		float vy = getMaxSpeed() * std::sin(radians);
		// change entity velocity
		setVelocity(vx, vy);
		// add to travelled distance
		nTravelledDistance += getMaxSpeed() * dt.asSeconds();
	}
}


// update Textnodes
void Character::updateTexts()
{
	nHealthDisplay->setString(toString(getHitpoints()) + " HP");
	nHealthDisplay->setPosition(0.f, - 50.f);		// -50 y above character

	// set rotation so text is always upright
	nHealthDisplay->setRotation(-getRotation());

//	std::cout << getRotation() << std::endl;
}


// get rect from world transform + sprite
sf::FloatRect Character::getBoundingRect() const
{
	/*sf::FloatRect result = getWorldTransform().transformRect(nSprite.getGlobalBounds());

	std::cout << result.height << "\n";

	std::cout << "Aaaa\n";*/

	return getWorldTransform().transformRect(nSprite.getGlobalBounds());
}

// public function, used to detect whetehr object is to be destroyed or no
bool Character::isMarkedForRemoval() const
{
	return nIsMarkedForRemoval;
}

bool Character::isAllied()const
{
	switch (nType)
	{
	case Character::DarkMagician:
		return true;
	default:
		return false;
	}
}

// buff character
void Character::increaseAttackRate()
{
	// if attack level not max
	if (nNormalAttackRateLevel < 10)
		nNormalAttackRateLevel++;
}

void Character::increaseSpread()
{
	if (nSpreadLevel < 3)
		nSpreadLevel++;
}

// add special attack
void Character::collectSpecialAttacks(unsigned int count)
{
	nSpecialAmount += count;
}

// change velocity from entity
void Character::modifyCharacterSpeed(float amount)
{
	nCharacterSpeed += amount;
}

float Character::getCharacterSpeed()
{
	return nCharacterSpeed;
}

// based on current attack type
void Character::launchAttack()
{
	if (CharacterTable[nType].actionInterval != sf::Time::Zero)
	{
		//		std::cout << "Flag set true\n";
		nIsLaunchingNormal = true;		// set flag to true
	}
}

// attack function, set flags to true
void Character::launchNormal()
{
	// std::cout << "Launched normal attack\n";
	nAttackType = NormalCircular;

	// only attack if interval not 0
	if (CharacterTable[nType].actionInterval != sf::Time::Zero)
	{
//		std::cout << "Flag set true\n";
		nIsLaunchingNormal = true;		// set flag to true
	}
}

void Character::launchSingle(AttackType type)
{
	nAttackType = type;

	if (CharacterTable[nType].actionInterval != sf::Time::Zero)
	{
		//		std::cout << "Flag set true\n";
		nIsLaunchingNormal = true;		// set flag to true
	}
}

void Character::launchSpecial()
{
	//std::cout << "Launched special attack\n";


	if (nSpecialAmount > 0)
	{
		//std::cout << "Launched special attack\n";

		nIsLaunchingSpecial = true;
		nSpecialAmount--;
	}
}


// change projectile type
void Character::changeProjectile()
{
	// only for allied characters
	if (isAllied())
	{
		std::vector<Projectile::Type> alliedTypes = {Projectile::AlliedSingle, Projectile::AlliedSingleBurst, 
			Projectile::AlliedSingleQuick};


		auto itr = std::find(alliedTypes.begin(), alliedTypes.end(), nCurrentProjectileType);

		assert(itr != alliedTypes.end());
//		std::cout << "proj changed\n";

		itr = std::next(itr, 1);
		if (itr == alliedTypes.end())
			itr = alliedTypes.begin();

	/*	if (*itr == Projectile::AlliedSingleBurst)
			std::cout << "changed to burst\n";

		if (*itr == Projectile::AlliedSingleQuick)
			std::cout << "changed to quick\n";*/

		nCurrentProjectileType = *itr;
	}
}


// randomly select if drop is a power pickup or no
void Character::checkPickupDrop(CommandQueue& commands)
{
	// if enemy and rnadom int matches
	if (!isAllied() && randomInt(3) == 0)
	{
		// push command
	//	commands.push(nDropPowerupCommand);
	}
}


// code implementation of launching an attack
void Character::checkProjectileLaunch(sf::Time dt, CommandQueue& commands)
{
	// enemy tries to fire all the time
	if (!isAllied())
		launchAttack();			// launch bsed on current attacktype


	// check from flag and cooldown must be 0
	if (nIsLaunchingNormal && nAttackCountdown <= sf::Time::Zero && nAttackType != NormalCircular)
	{
		// push the command
		commands.push(nLaunchNormalCommand);

		// add to cooldown
		nAttackCountdown += CharacterTable[nType].actionInterval / (nNormalAttackRateLevel + 1.f);
		nIsLaunchingNormal = false;		// set flag back to false
	}
	else if (nIsLaunchingNormal && nCircularAttackCountdown <= sf::Time::Zero)
	{
		commands.push(nLaunchNormalCommand);
		nCircularAttackCountdown = sf::seconds(1.0f);		// set cooldown to 2.5 seconds
		nIsLaunchingNormal = false;
	}
	else if (nAttackCountdown > sf::Time::Zero)		// else, decrease countdown
	{
		nAttackCountdown -= dt;
		nIsLaunchingNormal = false;
	}
	else if (nCircularAttackCountdown > sf::Time::Zero)
	{
		nCircularAttackCountdown -= dt;
		nIsLaunchingNormal = false;
	}
	

	// check for special attack
	if (nIsLaunchingSpecial)
	{
		commands.push(nLaunchSpecialCommand);
		nIsLaunchingSpecial = false;
	}
}





