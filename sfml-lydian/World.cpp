#include "World.hpp"
#include "ParticleNode.hpp"
#include "Projectile.hpp"
#include "Powerup.hpp"
#include "SoundNode.hpp"
#include "Player.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

#include <iostream>
#include <algorithm>		// std::sort
#include <limits>		// std::numeric_limits

// world constructor
World::World(sf::RenderTarget& outputTarget, FontHolder& fonts, SoundPlayer& sounds, Player& player) :
	nTarget(outputTarget),
	nWorldView(outputTarget.getDefaultView()),		// default window view, view covers whole window
	nTextures(),		// set these to empty first
	nFonts(fonts),
	nSounds(sounds),
//	nTileMap(nMap),
	nSceneGraph(),
	nSceneTexture(),
	nSceneLayers(),
	nActiveEnemies(),
	nWorldBounds(				// floatrect type
		0.f,	// left x position
		0.f,	// top y position
		4000.f,	  // view/entire window width
	//	nWorldView.getSize().y				//  height, for tile repeating
		4000.f
	),
	nSpawnPosition(
	//	nWorldView.getSize().x  / 2.f,			// middle x 
	//	nWorldView.getSize().y / 2.f			// bottom of the world minus half a screen height
		//nWorldBounds.height - nWorldView.getSize().y / 2.f
		640.f,
		360.f
	),
//	nNonPlayerSpawnPoints(),
	nPlayerCharacter(nullptr),
	nPlayer(&player)
{
	// create the render texture here
	nSceneTexture.create(nTarget.getSize().x, nTarget.getSize().y);

	loadTextures();		// load textures from resource holder

	buildMap();
	buildScene();		// build scene graph

	nWorldView.setCenter(nSpawnPosition);		// move view to correct start position
	nPlayer->setWorldView(nWorldView);
	nWorldView.zoom(0.65f);		// initial zoom
}


// loading textures into the resource holder
void World::loadTextures()
{
	nTextures.load(Textures::Void, "Media/Tilesets/Museum Interior.png");

	nTextures.load(Textures::DarkMagician, "Media/Textures/Magician-Girl-Down.png");
	nTextures.load(Textures::Shinobu, "Media/Textures/Shinobu-Resized.png");
	nTextures.load(Textures::Izuko, "Media/Textures/Izuko-Resized.png");
	nTextures.load(Textures::Hitagi, "Media/Textures/Hitagi-Resized.png");
	nTextures.load(Textures::Yotsugi, "Media/Textures/Yotsugi.png");

	//nTextures.load(Textures::Elesa, "Media/Characters/BW081.png");
	nTextures.load(Textures::Elesa, "Media/Characters/Elesa-75-Resized.png");

	nTextures.load(Textures::AlliedNormal, "Media/Textures/Pink-Flower-2.png");
	nTextures.load(Textures::SpecialHeart, "Media/Textures/Special-Heart-Cyan.png");
	nTextures.load(Textures::EnemyNormal, "Media/Textures/Pink-Flower.png");
	nTextures.load(Textures::AlliedSingle, "Media/Textures/Pink-Beam.png");
	nTextures.load(Textures::AlliedSingleBurst, "Media/Textures/Green-Plasma-Beam.png");
	nTextures.load(Textures::AlliedSingleQuick, "Media/Textures/Slim-Blue-Beam-Small.png");

	nTextures.load(Textures::CyanHeartBeam, "Media/Textures/Particle.png");
	nTextures.load(Textures::Explosion, "Media/Textures/Explosion.png");
}

void World::buildMap()
{
	auto map = std::make_unique<TileMap>();

	for (unsigned i = 0; i < 60 ; i++)
	{
		for (unsigned j = 0; j < 60; j++)
		{
			map->addTile(i, j, 0);
		}
	}

	nTileMap = map.get();
	nSceneLayers[Map] = map.get();
	nSceneGraph.attachChild(std::move(map));
}


// building the scene from the scenegraph
void World::buildScene()
{
	for (std::size_t i = 0; i < LayerCount; i++)
	{
		if (i == Map) continue;

		Category::Type category = (i == LowerVoid) ? Category::SceneVoidLayer : Category::None;

		SceneNode::Ptr layer = std::make_unique<SceneNode>(category);
		nSceneLayers[i] = layer.get();		

		nSceneGraph.attachChild(std::move(layer));			// attach to scene graph children directly below the root node
	}
	
	auto movement = std::make_unique<MovementComponent>();
	auto animation = std::make_unique<AnimationComponent>(*movement, nTextures.get(Textures::Elesa));

	auto main = std::make_unique<Entity>(400, std::move(movement), std::move(animation));
	nPlayerCharacter = main.get();			
	nPlayerCharacter->setPosition(nSpawnPosition);				
	nSceneLayers[LowerVoid]->attachChild(std::move(main));

	sf::Texture& texture = nTextures.get(Textures::Void);			
	std::unique_ptr<SpriteNode> backgroundSprite = std::make_unique<SpriteNode>(texture);	
	backgroundSprite->setPosition(nWorldBounds.left, nWorldBounds.top);		// set to top corner
	nSceneLayers[Map]->attachChild(std::move(backgroundSprite));

	std::unique_ptr<ParticleNode> cyanTrailNode = std::make_unique<ParticleNode>(Particle::CyanHeartBeam, nTextures);
	nSceneLayers[LowerVoid]->attachChild(std::move(cyanTrailNode));

	std::unique_ptr<SoundNode> soundNode = std::make_unique<SoundNode>(nSounds);
	nSceneGraph.attachChild(std::move(soundNode));

//	addNPCs();
}


// drawing the whole scene graph
void World::draw()
{
	if (!PostEffect::isSupported())
	{
		nSceneTexture.clear();
		nSceneTexture.setView(nWorldView);
		nSceneTexture.draw(nSceneGraph);
		nSceneTexture.display();
		nBloomEffect.apply(nSceneTexture, nTarget);
	}
	else		
 	{
		nTarget.setView(nWorldView);
		nTarget.draw(nSceneGraph);
	}
	
}


// updating the scene graph
void World::update(sf::Time dt)
{
	nPlayerCharacter->setVelocity(0.f, 0.f);
	
	updateTileMap();
	
	destroyEntitiesOutsideView();
	guideSpecialAttacks();

	// forward commands from queue to the scene graph
	while (!nCommandQueue.isEmpty())			
	{		
		nSceneGraph.onCommand(nCommandQueue.pop(), dt);			// pop form queue run command
	}

	adaptPlayerVelocity();		// slow down when in diagonal movement
	handleCollisions();

	nSceneGraph.removeWrecks();
	spawnNonPlayerCharacters();

	nSceneGraph.update(dt, nCommandQueue);			// update the scenegraph

//	adaptPlayerPosition();
	updateSounds();
}

void World::updateTileMap()
{
	// limit based on current center view
	unsigned gridSize = nTileMap->getGridSize();		// square a
	int startX, endX, startY, endY;

	int xSpan = nTarget.getSize().x / gridSize / 2 + 2;
	int ySpan = nTarget.getSize().y / gridSize / 2 + 2;

	// get based on view position
	startX = nWorldView.getCenter().x / gridSize - xSpan;
	endX = nWorldView.getCenter().x / gridSize + xSpan;
	startY = nWorldView.getCenter().y / gridSize - ySpan;
	endY = nWorldView.getCenter().y / gridSize + ySpan;

	std::cout << "CenterXY: " << nWorldView.getCenter().x << " " << nWorldView.getCenter().y << std::endl;

	int mapSize = nTileMap->getMapSize().x;			// square does not matter

	if (startX < 0)
		startX = 0;
	else if (startX >= mapSize)
		startX = mapSize - 1;

	if (endX < 0)
		endX = 0;
	else if (endX >= mapSize)
		endX = mapSize - 1;

	if (startY < 0)
		startY = 0;
	else if (startY >= mapSize)
		startY = mapSize - 1;

	if (endY < 0)
		endY = 0;
	else if (endY >= mapSize)
		endY = mapSize - 1;

	nTileMap->setRenderLimit(startX, endX, startY, endY);
}

sf::View* World::getWorldView()
{
	return &nWorldView;
}
TileMap* World::getTileMap()
{
	return nTileMap;
}

void World::updateSounds()
{
	nSounds.setListenerPosition(nPlayerCharacter->getWorldPosition());
	nSounds.removeStoppedSounds();					// remove stopped sounds from the linked list
}

void World::destroyEntitiesOutsideView()
{
	// make new command to destroy entities, push it to the queue
	Command command;
	// affects enemies and projectiles
	command.category = Category::Projectile | Category::EnemyCharacter;
	// set up the command, downcast to entity
	command.action = derivedAction<Entity>([this](Entity& entity, sf::Time)
	{
			if (!getFieldBounds().intersects(entity.getBoundingRect()))
			{
				entity.remove();
			}
	});

	nCommandQueue.push(command);
}


// command queue operations
CommandQueue& World::getCommandQueue()
{
	return nCommandQueue;
}


// for diagonal movement
void World::adaptPlayerVelocity()
{
	sf::Vector2f velocity = nPlayerCharacter->getVelocity();		// get current velocity

	if (velocity.x != 0.f && velocity.y != 0.f)				// if velocity not nil
		nPlayerCharacter->setVelocity(velocity / std::sqrt(2.f));		// get diagonal
}

// keep player within screen bounds
void World::adaptPlayerPosition()
{
	// get viewbound from world view
	sf::FloatRect viewBounds(nWorldView.getCenter() - nWorldView.getSize() / 2.f, nWorldView.getSize());
	const float borderDistance = 50.f;			

	// change character position
	sf::Vector2f position = nPlayerCharacter->getPosition();		// get current position
	position.x = std::max(position.x, viewBounds.left + borderDistance);
	position.x = std::min(position.x, viewBounds.left + viewBounds.width - borderDistance);
	position.y = std::max(position.y, viewBounds.top + borderDistance);
	position.y = std::min(position.y, viewBounds.top + viewBounds.height - borderDistance);
	nPlayerCharacter->setPosition(position);
}



// spawn points constructor
World::SpawnPoint::SpawnPoint(Character::Type type, float x, float y) :
	type(type), x(x), y(y)
{
}

// spawn npcs
void World::spawnNonPlayerCharacters()
{
	// while there are npcs to spawn
	// if npc y coordiante lies below the top member, create it at spawn point (within bounds)
	// .back() to access end of vector
	while (!nNonPlayerSpawnPoints.empty() && nNonPlayerSpawnPoints.back().y > getFieldBounds().top)
	{
		SpawnPoint spawn = nNonPlayerSpawnPoints.back();

		// create character
		std::unique_ptr<Character> npc = std::make_unique<Character>(spawn.type, nTextures, nFonts);
		npc->setPosition(spawn.x, spawn.y);
		// npc->setRotation(180.f);			// face down

		// attach to layer
		nSceneLayers[LowerVoid]->attachChild(std::move(npc));
		nNonPlayerSpawnPoints.pop_back();
	}
}


// getting bounds
sf::FloatRect World::getViewBounds() const
{
	return sf::FloatRect(nWorldView.getCenter() - nWorldView.getSize() / 2.f, nWorldView.getSize());
}


sf::FloatRect World::getFieldBounds() const
{
	// view bounds + some area top for entities to spawn
	sf::FloatRect bounds = getViewBounds();
	bounds.top -= 100.f;
	bounds.height += 100.f;
	return bounds;
}

// adding npcs
void World::addNPC(Character::Type type, float relX, float relY)		// based on nspawnposition
{
	// create spawn type
	SpawnPoint spawn(type, nSpawnPosition.x + relX, nSpawnPosition.y + relY);
	nNonPlayerSpawnPoints.push_back(spawn);			// push to vector
}

void World::addNPCs()
{
	
	//addNPC(Character::Izuko, -400.f, -100.f);
	//addNPC(Character::Izuko, -400.f, 50.f);
	//addNPC(Character::Izuko, -400.f, 200.f);
	//
	//addNPC(Character::Shinobu, -400.f, -200.f);
	//addNPC(Character::Shinobu, -400.f, 0.f);
	//addNPC(Character::Shinobu, -400.f, 200.f);

	//addNPC(Character::Hitagi, -200.f, -400.f);
	//addNPC(Character::Hitagi, 0.f, -400.f);
	//addNPC(Character::Hitagi, 200.f, -400.f);
	//

	//addNPC(Character::Shinobu, -200.f, 200.f);
	 

	addNPC(Character::Yotsugi, -300.f, 300.f);
	addNPC(Character::Yotsugi, 0.f, 300.f);
	addNPC(Character::Yotsugi, 300.f, 300.f);
	addNPC(Character::Yotsugi, -300.f, -300.f);
	addNPC(Character::Yotsugi, 0.f, -300.f);
	addNPC(Character::Yotsugi, 300.f, -300.f);

	addNPC(Character::Yotsugi, -300.f, -300.f);
	addNPC(Character::Yotsugi, -300.f, 0.f);
	addNPC(Character::Yotsugi, -300.f, 300.f);
	addNPC(Character::Yotsugi, 300.f, -300.f);
	addNPC(Character::Yotsugi, 300.f, 0.f);
	   

	// sort according to y values, lower enemis are checked first
	std::sort(nNonPlayerSpawnPoints.begin(), nNonPlayerSpawnPoints.end(), [](SpawnPoint lhs, SpawnPoint rhs)
		{
			return lhs.y < rhs.y;
		});
}


void World::guideSpecialAttacks()
{
	Command enemyCollector;
	enemyCollector.category = Category::EnemyCharacter;

	// function to collect all currently alive enemies
	// derived action to downcast from node type to Character type, use inline function
	enemyCollector.action = derivedAction<Character>(
		[this](Character& enemy, sf::Time)
		{
			if(!enemy.isDestroyed())	// if enemy is still alive
				nActiveEnemies.push_back(&enemy);
		});


	// function that guides the special attacks to enemies which are closest to player
	Command specialGuider;
	specialGuider.category = Category::AlliedProjectile;
	specialGuider.action = derivedAction<Projectile>([this](Projectile& projectile, sf::Time)
	{
			// ignore non-guided projectiles
			if (!projectile.isGuided())
				return;

			/*	std::numeric_limits
			get, eg. maximum number of a type
			std::numeric_limits<type>::max();
			*/
			float minDistance = std::numeric_limits<float>::max();  // get max float amount
			Character* closestEnemy = nullptr;

			// find enemiees from active enemies
			for (Character*& enemy : nActiveEnemies)
			{	
				// float enemyDistance = 10;
				float enemyDistance = distance(projectile, *enemy);			// scenenode function to get distance between two nodes

				if (enemyDistance < minDistance)
				{
					closestEnemy = enemy;
					minDistance = enemyDistance;
				}
			}

			if (closestEnemy)	// guide projectile
				projectile.guideTowards(closestEnemy->getWorldPosition());
	});

	// push commands
	nCommandQueue.push(enemyCollector);
	nCommandQueue.push(specialGuider);
	nActiveEnemies.clear();			// clear active enemies
}


// check if match parameter types
static bool matchesCategories(SceneNode::Pair& colliders, Category::Type first, Category::Type second)
{
	unsigned int firstCategory = colliders.first->getCategory();
	unsigned int secondCategory = colliders.second->getCategory();

	// check both cases, use & bitwise operator
	if (first & firstCategory && second & secondCategory)
	{
		return true;
	}
	else if (second & firstCategory && first & secondCategory)
	{
		// swap to match category
		std::swap(colliders.first, colliders.second);
		return true;
	}
	else
	{
		return false;
	}
}

void World::handleCollisions()
{
	// set up collision pair set
	std::set<SceneNode::Pair> collisionPairs;
	// get all pairs from the main scenegraph
	nSceneGraph.checkSceneCollision(nSceneGraph, collisionPairs);

	for (SceneNode::Pair pair : collisionPairs)
	{
		/* implemenation of matching collisions here */

		// betweenplaye and enemy character
		if (matchesCategories(pair, Category::PlayerCharacter, Category::EnemyCharacter))
		{
			auto& player = static_cast<Character&>(*pair.first);
			auto& enemy = static_cast<Character&>(*pair.second);

			player.damage(enemy.getHitpoints());			// damage equal to enemy health
			enemy.destroy();
		}
		else if (matchesCategories(pair, Category::AlliedProjectile, Category::EnemyCharacter)
			|| matchesCategories(pair, Category::EnemyProjectile, Category::PlayerCharacter))
		{
			auto& projectile = static_cast<Projectile&>(*pair.first);
			auto& character = static_cast<Character&>(*pair.second);
			
			character.damage(projectile.getDamage());			// damage enemy based on projectile
			projectile.destroy();					// destroy projectile
		}
		else if (matchesCategories(pair, Category::PlayerCharacter, Category::Powerup))
		{
			auto& player = static_cast<Character&>(*pair.first);
			auto& powerup = static_cast<Powerup&>(*pair.second);

			powerup.apply(player);
			powerup.destroy();
		}
	}

}


// mission status
bool World::hasAlivePlayer() const
{
//	return !nPlayerCharacter->isMarkedForRemoval();

	return true;
}

bool World::gameReachedEnd() const
{
	// character within world bounds
//	return !nWorldBounds.contains(nPlayerCharacter->getPosition());

	return false;
}