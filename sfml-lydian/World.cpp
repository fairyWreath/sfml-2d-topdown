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

	// add bacgkround sprite to the scene, with SpriteNode class
	// pass in texture and texturerect to SpriteNode constructor
	std::unique_ptr<SpriteNode> backgroundSprite = std::make_unique<SpriteNode>(texture, textureRect);	
	backgroundSprite->setPosition(nWorldBounds.left, nWorldBounds.top);		// set to top corner
	nSceneLayers[Background]->attachChild(std::move(backgroundSprite));		// move the ptr and attach it to background layer 

	// add the user character
	// pass in character type and texture holder
	std::unique_ptr<Character> main = std::make_unique<Character>(Character::DarkMagician, nTextures);
	nPlayerCharacter = main.get();			// get RAW  C pointer from unique_ptr
	nPlayerCharacter->setPosition(nSpawnPosition);			// set to spawn position
	nPlayerCharacter->setVelocity(40.f, nScrollSpeed);		// set velocity
	// add unique_ptr of the character to 2nd scene layer
	nSceneLayers[Void]->attachChild(std::move(main));


	// add other textures, eg. escort characters
	// can attach them to nPlayercharacter node
}

// drawing the scene graph, expensive operation
void World::draw()
{
	nWindow.setView(nWorldView);			// configure view
	nWindow.draw(nSceneGraph);			// use overidden SceneNode method to draw the whole scene graph
}


// updating the scene graph
void World::update(sf::Time dt)
{
	nWorldView.move(0.f, nScrollSpeed * dt.asSeconds());		// move the view

	// get character position/velocity
	sf::Vector2f position = nPlayerCharacter->getPosition();	// from sf::Transformable
	sf::Vector2f velocity = nPlayerCharacter->getVelocity();		// also method from sf::Transformable

	if (position.x <= nWorldBounds.left + 150
		|| position.x >= nWorldBounds.left + nWorldBounds.width - 150)		// more/less than 150 on each side
	{
		velocity.x = -velocity.x;		// change character velocity
		nPlayerCharacter->setVelocity(velocity);
	}

	nSceneGraph.update(dt);			// update the scenegraph
}



