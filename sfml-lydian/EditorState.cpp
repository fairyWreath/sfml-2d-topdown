#include "EditorState.hpp"
#include "World.hpp"
#include "Player.hpp"
#include "Utility.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

#include <iostream>
#include <memory>

using namespace std::placeholders;

EditorState::EditorState(StateStack& stack, Context context) :
	State(stack, context),
	nWorld(context.world),
	nWindow(*context.window),
	nPlayer(context.player),
	nTileMap(context.world->getTileMap()),
	nScrollSpeed(400.f),
	nZoomSpeed(0.8f),
	nWorldView(nWorld->getWorldView())
{
	context.musicPlayer->play(Music::SecondaryTheme);

	nGridSelector.setSize(sf::Vector2f((float)nTileMap->getGridSize(), (float)nTileMap->getGridSize()));
	nGridSelector.setFillColor(sf::Color::Transparent);
	nGridSelector.setOutlineThickness(1.5f);
	nGridSelector.setOutlineColor(sf::Color::Magenta);

	nMousePosViewText.setFont(context.fonts->get(Fonts::Main));
	nMousePosViewText.setString("");
	nMousePosViewText.setCharacterSize(12);
	nMousePosViewText.setFillColor(sf::Color::Magenta);
//	centerOrigin(nMousePosViewText);
	nMousePosViewText.setPosition(0.f, 0.f);
}

void EditorState::draw()
{
	nWorld->draw();

	// draw the grid selector above the world
	nWindow.draw(nGridSelector);
	nWindow.draw(nMousePosViewText);

//	nWindow.setView(nWindow.getDefaultView());
}

bool EditorState::update(sf::Time dt)
{
	updateMouseText();

	handleRealtimeInput(dt);

	nWorld->update(dt);

	// update player realtime input
	CommandQueue& commands = nWorld->getCommandQueue();
	nPlayer->handleRealtimeInput(commands);

	return true;
}

void EditorState::updateMouseText()
{
	sf::Vector2i mousePosition = sf::Mouse::getPosition(nWindow);
	nWindow.setView(*nWorldView);
	sf::Vector2f worldPosition = nWindow.mapPixelToCoords(mousePosition);
	nMousePosViewText.setPosition((float)worldPosition.x, (float)worldPosition.y + 25);

	std::string text = std::to_string((int)worldPosition.x) + ", " + std::to_string((int)worldPosition.y);
	nMousePosViewText.setString(text);
}

void EditorState::initializeActions()
{
	
}

void EditorState::handleRealtimeInput(sf::Time dt)
{
	// can set custom keybindings as well
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		nWorldView->move(nScrollSpeed * dt.asSeconds(), 0.f);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		nWorldView->move(0.f , -nScrollSpeed * dt.asSeconds());
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		nWorldView->move(-nScrollSpeed * dt.asSeconds(), 0.f);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		nWorldView->move(0.f, nScrollSpeed * dt.asSeconds());
	}

	// mouse events, keep mouse within window and scroll
	int maxX = nWindow.getSize().x;
	int maxY = nWindow.getSize().y;

	int mX = sf::Mouse::getPosition(nWindow).x;
	int mY = sf::Mouse::getPosition(nWindow).y;


	if (mX < 0 || mY < 0 || mX > maxX || mY > maxY)
	{
		if (mX < 0)
		{
			mX = 0;
			nWorldView->move(-nScrollSpeed * dt.asSeconds(), 0.f);
		}
		else if (mX > maxX)
		{
			mX = maxX;
			nWorldView->move(nScrollSpeed * dt.asSeconds(), 0.f);
		}

		if (mY < 0)
		{
			mY = 0;
			nWorldView->move(0.f, -nScrollSpeed * dt.asSeconds());
		}
		else if (mY > maxY)
		{
			mY = maxY;
			nWorldView->move(0.f, nScrollSpeed * dt.asSeconds());
		}

	//	sf::Mouse::setPosition(sf::Vector2i(mX, mY), nWindow);
	}
}


bool EditorState::handleEvent(const sf::Event& event)
{
	// handle player events
	CommandQueue& commands = nWorld->getCommandQueue();
	nPlayer->handleEvent(event, commands);


	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
	{
		requestStackPush(States::Pause);
	}

	// always update?
	if (event.type == sf::Event::MouseMoved || true)
	{
		sf::Vector2i mousePosition = sf::Mouse::getPosition(nWindow);

		/* set view again for window before getting coords */
		nWindow.setView(*nWorldView);
		sf::Vector2f worldPosition = nWindow.mapPixelToCoords(mousePosition);
	
		// based on grid
		sf::Vector2u mousePosGrid;
		
		unsigned gridSize = nTileMap->getGridSize();

		if (worldPosition.x >= 0.f)
			mousePosGrid.x = (unsigned)worldPosition.x / nTileMap->getGridSize();
		if (worldPosition.y >= 0.f)
			mousePosGrid.y = (unsigned)worldPosition.y / nTileMap->getGridSize();
	
		// set tile position
		nGridSelector.setPosition((float)mousePosGrid.x * gridSize, (float)mousePosGrid.y * gridSize);

		if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
		{
			nTileMap->addTile(mousePosGrid.x, mousePosGrid.y, 0);
			std::cout << "Grid Pos: " << mousePosGrid.x << " " << mousePosGrid.y << std::endl;
		}
		if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Right)
			nTileMap->removeTile(mousePosGrid.x, mousePosGrid.y, 0);
	}

	// mouse zoom in and zoom out
	if (event.type == sf::Event::MouseWheelMoved)
	{
		if (event.mouseWheel.delta > 0)
			nWorldView->zoom(nZoomSpeed);
		else if (event.mouseWheel.delta < 0)
			nWorldView->zoom(1 / nZoomSpeed);
	}


	nGUIContainer.handleEvent(event);

	return true;
}
