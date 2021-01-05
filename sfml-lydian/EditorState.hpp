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
	void initializeTexts();

	void handleRealtimeInput(sf::Time dt);

	void updateMouseText();
	void updateGridSelector();
	void updateCurrentTile();			// not used
	void updateTextureSelector();

private:
	std::map<EditAction, EditCommand> nActionBindings;

	sf::RenderWindow& nWindow;

	// grid selector
	sf::RectangleShape nGridSelector;
	sf::Text nMousePosViewText;
	sf::Sprite nCurrentTile;			// not used

	// change to object references later maybe?
	World* nWorld;
	sf::View* nWorldView;		// to move around the world
	TileMap* nTileMap;

	// currently selected tilesheet for the tile map builder
	Tiles::ID nCurrentTileID;

	// delete later
	sf::Vector2f nTextureSelectorPosition;		// texture selector window position

	Player* nPlayer;
	TextureHolder& nTextures;

	float nScrollSpeed;
	float nZoomSpeed;


	// update slowly
	sf::Time nElapsedTime;


	GUI::Container nGUIContainer;
};


#endif