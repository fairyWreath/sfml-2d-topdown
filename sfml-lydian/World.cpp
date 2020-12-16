#include "World.hpp"

// world constructor
World::World(sf::RenderWindow& window) :
	nWindow(window),
	nWorldView(window.getDefaultView()),		// default window view, view covers whole window
	nTextures(),		// set these to empty first
	nSceneGraph(),	
	nSceneLayers(),
	nWorldBounds(				// floatrect type
		0.f,	// left x position
		0.f,	// top y position
		nWorldView.getSize().x,	  // view/entire window width
		2000.f				//  height, for tile repeating
	),
	nSpawnPosition(
		nWorldView.getSize().x  / 2.f,			// middle x 
		nWorldBounds.height - nWorldView.getSize().y / 2.f			// bottom of the world minus half a screen height
	),
	nScrollSpeed(-50.f),		// set scroll speed -50 float units 
	nPlayerCharacter(nullptr)		// set nullptr to character
{
	loadTextures();		// load textures from resource holder
	buildScene();		// build scene graph

	nWorldView.setCenter(nSpawnPosition);		// move view to correct start position
}


// loading from textures
void World::loadTextures()
{
	// loading from textures class template, load with identifier and filepath
	nTextures.load(Textures::Void, "Media/Textures/Void.png");
	nTextures.load(Textures::DarkMagician, "Media/Textures/Magician-Girl-Down.png");
}


// building the scene from the scenegraph
void World::buildScene()
{
	// first initialize different scene layers based on layercount
	// add scenes to the scenelayer array
	for (std::size_t i = 0; i < LayerCount; i++)
	{
		SceneNode::Ptr layer = std::make_unique<SceneNode>();			// initialize new scenenode
		//  attach to SceneNode* array, with std::unique_ptr.get(), passing it to RAW C POINTER; the unique_ptr still POINTS/works
		nSceneLayers[i] = layer.get();		

		nSceneGraph.attachChild(std::move(layer));			// attach to scene graph children directly below the root node
	}


	// after the background is loaded, configure the tile to repeat itself
	sf::Texture& texture = nTextures.get(Textures::Void);			// get memory address of background texture
	sf::IntRect textureRect(nWorldBounds);				// texture rect with int coordinates, converted from worldbounds
	texture.setRepeated(true);				// repeate the tile



}

