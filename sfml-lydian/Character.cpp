#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

#include "Character.hpp"
#include "ResourceHolder.hpp"
#include "Category.hpp"			// return category for commands

#include "Utility.hpp"

#include "DataTables.hpp"

#include "Command.hpp"
#include "CommandQueue.hpp"

#include <cmath>

#include <iostream>

namespace
{
	const std::vector<CharacterData> CharacterTable = initializeCharacterData();
}

// map corresponding character type to Textures::ID
static Textures::ID toTextureID(Character::Type type)
{
	switch (type)
	{
	case Character::DarkMagician:
		return Textures::DarkMagician;
	
	case Character::Shinobu:
		return Textures::Shinobu;

	case Character::Izuko:
		return Textures::Izuko;

	case Character::Hitagi:
		return Textures::Hitagi;
	}
	
	// default
	return Textures::DarkMagician;
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
	nNormalAttackRateLevel(1),
	nSpreadLevel(1),
	nSpecialAmount(2),
	nAttackCountdown(sf::Time::Zero),
	nIsMarkedForRemoval(false),
	nSprite(textures.get(toTextureID(type)))		// get sprite from texture id type
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
}

// creating attacks
void Character::createNormalAttack(SceneNode& node, const TextureHolder& textures) const
{
	// check if ally or enemy
	Projectile::Type type = isAllied() ? Projectile::AlliedNormal : Projectile::EnemyNormal;

	// create based on spread level
	
	switch (nSpreadLevel)
	{
	case 1:
		createProjectile(node, type, 0.0f, 0.5f, textures);
		break;
	default:
		createProjectile(node, type, 0.0f, 0.5f, textures);
		break;
	}

}


// creating a single projectile
void Character::createProjectile(SceneNode& node, Projectile::Type type, float xOffset, 
	float yOffset, const TextureHolder& textures) const
{
	// create projectile ptr
	std::unique_ptr<Projectile> projectile = std::make_unique<Projectile>(type, textures);

	std::cout << "created projectile\n";

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

	// apply velocity, call virtual parent class function here
	Entity::updateCurrent(dt, commands);		

	// update text nodes
	updateTexts();
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
sf::FloatRect Character::getBoudingRect() const
{
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

// attack function, set flags to true
void Character::launchNormal()
{
	std::cout << "Launched normal attack\n";

	// only attack if interval not 0
	if (CharacterTable[nType].actionInterval != sf::Time::Zero)
	{
		std::cout << "Flag set true\n";
		nIsLaunchingNormal = true;		// set flag to true
	}
}

void Character::launchSpecial()
{
	if (nSpecialAmount > 0)
	{
		nIsLaunchingSpecial = true;
		nSpecialAmount--;
	}
}


// randomly select if drop is a power pickup or no
void Character::checkPickupDrop(CommandQueue& commands)
{
	// if enemy and rnadom int matches
	if (!isAllied() && randomInt(3) == 0)
	{
		// push command
		commands.push(nDropPickupCommand);
	}
}


// code implementation of launching an attack
void Character::checkProjectileLaunch(sf::Time dt, CommandQueue& commands)
{
	// enemy tries to fire all the time
	if (!isAllied())
		launchNormal();


	// check from flag and cooldown must be 0
	if (nIsLaunchingNormal && nAttackCountdown <= sf::Time::Zero)
	{
		std::cout << "Command pushed\n";

		// push the command
		commands.push(nLaunchNormalCommand);

		// add to cooldown
		nAttackCountdown += sf::seconds(1.f / (nNormalAttackRateLevel + 1));
		nIsLaunchingNormal = false;		// set flag back to false
	}
	else if (nAttackCountdown > sf::Time::Zero)		// else, decrease countdown
	{
		nAttackCountdown -= dt;
	}

	// check for special attack
	if (nIsLaunchingSpecial)
	{
		commands.push(nLaunchSpecialCommand);
		nIsLaunchingSpecial = false;
	}
}





