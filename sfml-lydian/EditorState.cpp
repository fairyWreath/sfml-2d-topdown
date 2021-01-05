#include "EditorState.hpp"
#include "World.hpp"
#include "Player.hpp"
#include "Utility.hpp"
#include "TileData.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

#include <iostream>
#include <memory>

using namespace std::placeholders;

namespace
{
	const sf::Time UpdateTime = sf::seconds(0.5f);

	const std::vector<TileSheetData> SheetData = initializeTileSheetData();
}

EditorState::EditorState(StateStack& stack, Context context) :
	State(stack, context),
	nWorld(context.world),
	nWindow(*context.window),
	nPlayer(context.player),
	nTextures(nWorld->getWorldTextures()),
	nTileMap(nWorld->getTileMap()),
	nScrollSpeed(1500.f),
	nZoomSpeed(0.8f),
	nWorldView(nWorld->getWorldView()),
	nCurrentTileID(Tiles::TallGrass2),
	nElapsedTime(sf::Time::Zero)
{
	context.musicPlayer->play(Music::SecondaryTheme);

	nGridSelector.setSize(sf::Vector2f((float)nTileMap->getGridSize(), (float)nTileMap->getGridSize()));
	nGridSelector.setOutlineThickness(1.5f);
	nGridSelector.setOutlineColor(sf::Color::Magenta);

	nMousePosViewText.setFont(context.fonts->get(Fonts::Main));
	nMousePosViewText.setString("");
	nMousePosViewText.setCharacterSize(18);
	nMousePosViewText.setFillColor(sf::Color::Magenta);
	nMousePosViewText.setPosition(0.f, 0.f);

	// initialize container for texture selector
	nGUIContainer.setBackground(true);
	sf::Color color = sf::Color::Magenta;
	color.a = sf::Uint8(70);
	nGUIContainer.setBackgroundColor(color);
	nGUIContainer.setBackgroundOutlineColor(sf::Color::Cyan);
	nGUIContainer.setBackgroundSize(300.f, 550.f);
	nGUIContainer.setBackgroundPosition(900.f, 100.f);

	nTextureSelectorPosition = sf::Vector2f(900.f, 100.f);

	updateMouseText();
	updateGridSelector();
}

void EditorState::draw()
{
	nWorld->draw();

	nWindow.draw(nGridSelector);
	nWindow.draw(nMousePosViewText);
	nWindow.draw(nCurrentTile);

	nWindow.draw(nGUIContainer);
}

bool EditorState::update(sf::Time dt)
{
	updateMouseText();
	updateGridSelector();
	updateTextureSelector();

	handleRealtimeInput(dt);

	nWorld->update(dt);

	CommandQueue& commands = nWorld->getCommandQueue();
	nPlayer->handleRealtimeInput(commands);

	return true;
}

void EditorState::updateMouseText()
{
	nWindow.setView(*nWorldView);

	sf::Vector2i mousePosition = sf::Mouse::getPosition(nWindow);
	sf::Vector2f worldPosition = nWindow.mapPixelToCoords(mousePosition);
	nMousePosViewText.setPosition((float)worldPosition.x, (float)worldPosition.y + 25);

	std::string text = std::to_string((int)worldPosition.x) + "\n" + std::to_string((int)worldPosition.y);
	nMousePosViewText.setString(text);
}

void EditorState::updateGridSelector()
{
	sf::Vector2i mousePosition = sf::Mouse::getPosition(nWindow);
	sf::Vector2f worldPosition = nWindow.mapPixelToCoords(mousePosition);
	sf::Vector2u mousePosGrid;

	unsigned gridSize = nTileMap->getGridSize();

	if (worldPosition.x >= 0.f)
		mousePosGrid.x = (unsigned)worldPosition.x / nTileMap->getGridSize();
	if (worldPosition.y >= 0.f)
		mousePosGrid.y = (unsigned)worldPosition.y / nTileMap->getGridSize();

	nGridSelector.setTexture(&nTextures.get(SheetData[nCurrentTileID].textureID));
	nGridSelector.setTextureRect(SheetData[nCurrentTileID].textureRect);
	nGridSelector.setPosition((float)mousePosGrid.x * gridSize, (float)mousePosGrid.y * gridSize);
}

void EditorState::updateTextureSelector()
{
	nWindow.setView(*nWorldView);
	sf::Vector2f worldPos = nWindow.mapPixelToCoords(static_cast<sf::Vector2i>(nTextureSelectorPosition));
	nGUIContainer.setBackgroundPosition(worldPos.x, worldPos.y);
}



// not used
void EditorState::updateCurrentTile()
{
	nCurrentTile.setTexture(nTextures.get(SheetData[nCurrentTileID].textureID));
	nCurrentTile.setTextureRect(SheetData[nCurrentTileID].textureRect);

	sf::Vector2i mousePosition = sf::Mouse::getPosition(nWindow);
	nWindow.setView(*nWorldView);
	sf::Vector2f worldPosition = nWindow.mapPixelToCoords(mousePosition);
	nCurrentTile.setPosition((float)worldPosition.x, (float)worldPosition.y + 25);
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

	nElapsedTime += dt;

	// change current tile 
	if (nElapsedTime > UpdateTime)
	{
		nElapsedTime -= UpdateTime;

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
		{
			nCurrentTileID = static_cast<Tiles::ID>(((int)nCurrentTileID + 1) % (int)Tiles::TileCount);
		}
	}

	// mouse events, keep mouse within window and scroll
	int maxX = nWindow.getSize().x;
	int maxY = nWindow.getSize().y;

	int mX = sf::Mouse::getPosition(nWindow).x;
	int mY = sf::Mouse::getPosition(nWindow).y;

	// move world when mouse is at edges
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
	}
}


bool EditorState::handleEvent(const sf::Event& event)
{
	CommandQueue& commands = nWorld->getCommandQueue();
	nPlayer->handleEvent(event, commands);


	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
	{
		requestStackPush(States::Pause);
	}

	if (event.type == sf::Event::MouseButtonPressed)
	{
		sf::Vector2i mousePosition = sf::Mouse::getPosition(nWindow);

		/* set view again for window before getting coords */
		nWindow.setView(*nWorldView);
		sf::Vector2f worldPosition = nWindow.mapPixelToCoords(mousePosition);

		sf::Vector2u mousePosGrid;
		unsigned gridSize = nTileMap->getGridSize();

		if (worldPosition.x >= 0.f)
			mousePosGrid.x = (unsigned)worldPosition.x / nTileMap->getGridSize();
		if (worldPosition.y >= 0.f)
			mousePosGrid.y = (unsigned)worldPosition.y / nTileMap->getGridSize();
		

		if (event.mouseButton.button == sf::Mouse::Left)
		{
			nTileMap->addTile(mousePosGrid.x, mousePosGrid.y, 0, nCurrentTileID);
			std::cout << "Grid Pos: " << mousePosGrid.x << " " << mousePosGrid.y << std::endl;
		}
		if (event.mouseButton.button == sf::Mouse::Right)
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

void EditorState::initializeActions()
{

}

void EditorState::initializeTexts()
{

}