#ifndef EDITORSTATE_HPP
#define EDITORSTATE_CPP

#include "State.hpp"
#include "Container.hpp"
#include "SceneNode.hpp"
#include "TileIdentifiers.hpp"

#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include <functional>

class EditorState : public State
{
public:
	typedef std::function<void()> EditCommand;

	enum EditAction
	{
		AddTile,
		ActionCount
	};

public:
	// constructor, pass in State class parameters
	EditorState(StateStack& stack, Context context);

	// virtual function for State class to be overidden
	virtual void draw();
	virtual bool update(sf::Time dt);
	virtual bool handleEvent(const sf::Event& event);

private:

	void initializeActions();

	void handleRealtimeInput(sf::Time dt);

	void updateMouseText();

private:
	std::map<EditAction, EditCommand> nActionBindings;

	sf::RenderWindow& nWindow;

	// grid selector
	sf::RectangleShape nGridSelector;

	// mouse pos view label
	sf::Text nMousePosViewText;

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