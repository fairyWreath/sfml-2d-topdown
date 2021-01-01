#ifndef WORLD_HPP
#define WORLD_HPP

// class for data related to rendering and logical data

#include "ResourceHolder.hpp"			// resource holder template class
#include "ResourceIdentifiers.hpp"	
#include "SceneNode.hpp"				// for scene graph
#include "SpriteNode.hpp"				// sprite scenenodes for landscape/background
#include "Character.hpp"
#include "Powerup.hpp"
#include "CommandQueue.hpp"
#include "BloomEffect.hpp"
#include "SoundPlayer.hpp"
#include "TileMap.hpp"

#include <SFML/System/NonCopyable.hpp>
#include <SFML/Graphics/View.hpp>			// view/viewports
#include <SFML/Graphics/Texture.hpp>

#include <array>		// statistically fixed sized array

// forward declare
namespace sf
{
	class RenderWindow;
}

// inherit world from sf::NonCopyable, prevents copy constructor
class World : private sf::NonCopyable
{
public:
	explicit World(sf::RenderTarget& outputTarget, FontHolder& fonts, SoundPlayer& sounds);			// pass in render window in constructor
	void update(sf::Time dt);		// update scenes
	void draw();					// draw sprites(expensive operation)

	// acess commandqueue memory address for writing
	CommandQueue& getCommandQueue();

	// end game states
	bool hasAlivePlayer() const;
	bool gameReachedEnd() const;

private:
	void loadTextures();		// load textures/sprites from resourceholder
	void buildScene();			// draw the scenes


	// build tile map
	void buildMap();

	void adaptPlayerVelocity();			// adapt player velocity for eg.diagonal movements
	void adaptPlayerPosition();			// keep player within screen bounds


	// collisions
	void handleCollisions();

	void updateSounds();

	// spawn all enemies
	void spawnNonPlayerCharacters();

	// adding npcs
	void addNPC(Character::Type type, float relX, float relY);
	void addNPCs();		// add a lot at once, hard coded


	// destroy entities outside the view
	void destroyEntitiesOutsideView();

	// guiding special missiles
	void guideSpecialAttacks();


	// float rect for bounds, used in spawning
	sf::FloatRect getViewBounds() const;
	sf::FloatRect getFieldBounds() const;

private:
	enum Layer					// scene layering
	{
		Background,				
		Map,
		LowerVoid,						// lowest layer
		LayerCount,
	};

	// structure for determining spawn points
	struct SpawnPoint
	{
		// character type and location of the spawns
		SpawnPoint(Character::Type type, float x, float y);

		Character::Type type;
		float x;
		float y;
	};

private:
//	sf::RenderWindow& nWindow;		// memory address of window, gained from game class later on
	
	sf::RenderTarget& nTarget;
	sf::RenderTexture nSceneTexture;

	sf::View nWorldView;			// world view

	TextureHolder nTextures;		// texture holder template class to hold sprites/textures
	FontHolder& nFonts;
	SceneNode nSceneGraph;			// root node of the scene graph

	// std::array allows value semantics, which allow copies, assignments, passing or returning objects from functions
	std::array<SceneNode*, LayerCount> nSceneLayers;		// fixed array size <typename, size_t> for scene layering

	sf::FloatRect nWorldBounds;			// rectaungular bounds
	sf::Vector2f nSpawnPosition;		// starting spawn position

	float nScrollSpeed;					// scroll speed of moving tyle
	Character* nPlayerCharacter;		// pointer to player character

	TileMap* nTileMap;

	// enemy spawn points
	std::vector<SpawnPoint> nNonPlayerSpawnPoints;

	// active/alive enemis
	std::vector<Character*> nActiveEnemies;

	// command queue to execute commands
	CommandQueue nCommandQueue;

	SoundPlayer& nSounds;

	BloomEffect nBloomEffect;
};




#endif