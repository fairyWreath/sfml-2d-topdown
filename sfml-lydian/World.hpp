#ifndef WORLD_HPP
#define WORLD_HPP

// class for data related to rendering and logical data

#include "ResourceHolder.hpp"			// resource holder template class
#include "ResourceIdentifiers.hpp"	
#include "SceneNode.hpp"				// for scene graph
#include "SpriteNode.hpp"				// sprite scenenodes for landscape/background
#include "Character.hpp"


#include <SFML/System/NonCopyable.hpp>
#include <SFML/Graphics/View.hpp>			// view/viewports
#include <SFML/Graphics/Texture.hpp>

#include <array>		// statistically fixed sized array

// inherit world from sf::NonCopyable, prevents copy constructor
class World : private sf::NonCopyable
{
public:
	explicit World(sf::RenderWindow& window);			// pass in render window in constructor
	void update(sf::Time dt);		// update scenes
	void draw();					// draw sprites(expensive operation)

private:
	void loadTextures();		// load textures/sprites from resourceholder
	void buildScene();			// draw the scenes

private:
	enum Layer					// scene layering
	{
		Background,					// highest layer, all children here are in 'one' layer class
		Void,						// lowest layer
		LayerCount,
	};

private:
	sf::RenderWindow& nWindow;		// memory address of window, gained from game class later on
	sf::View nWorldView;			// world view

	TextureHolder nTextures;		// texture holder template class to hold sprites/textures
	SceneNode nSceneGraph;			// root node of the scene graph


	// std::array allows value semantics, which allow copies, assignments, passing or returning objects from functions
	std::array<SceneNode*, LayerCount> nSceneLayers;		// fixed array size <typename, size_t> for scene layering

	sf::FloatRect nWorldBounds;			// rectaungular bounds
	sf::Vector2f nSpawnPosition;		// starting spawn position

	float nScrollSpeed;					// scroll speed of moving tyle
	Character* nPlayerCharacter;		// pointer to player character

};




#endif