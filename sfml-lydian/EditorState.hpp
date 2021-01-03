#ifndef EDITORSTATE_HPP
#define EDITORSTATE_CPP

#include "State.hpp"
#include "Container.hpp"
#include "SceneNode.hpp"

#include <SFML/Graphics/Sprite.hpp>

class EditorState : public State
{
public:
	// constructor, pass in State class parameters
	EditorState(StateStack& stack, Context context);

	// virtual function for State class to be overidden
	virtual void draw();
	virtual bool update(sf::Time dt);
	virtual bool handleEvent(const sf::Event& event);

	void handleRealtimeInput(sf::Time dt);

private:
	sf::RenderWindow& nWindow;

	// grid selector
	sf::RectangleShape nGridSelector;

	// change to object references later maybe?
	World* nWorld;
	sf::View* nWorldView;		// to move around the world
	TileMap* nTileMap;

	Player* nPlayer;

	float nScrollSpeed;
	float nZoomSpeed;

	// GUI container
	GUI::Container nGUIContainer;
};


#endif