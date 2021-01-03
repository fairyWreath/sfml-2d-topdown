#include "EditorState.hpp"
#include "World.hpp"
#include "Player.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

#include <iostream>
#include <memory>

EditorState::EditorState(StateStack& stack, Context context) :
	State(stack, context),
	nWorld(context.world),
	nWindow(*context.window),
	nPlayer(context.player),
	nScrollSpeed(400.f),
	nZoomSpeed(0.8f),
	nBackgroundSprite(),
	nWorldView(nWorld->getWorldView())
{
	context.musicPlayer->play(Music::SecondaryTheme);
}

void EditorState::draw()
{
	nWorld->draw();
}

bool EditorState::update(sf::Time dt)
{
	handleRealtimeInput(dt);

	nWorld->update(dt);

	// update player realtime input
	CommandQueue& commands = nWorld->getCommandQueue();
	nPlayer->handleRealtimeInput(commands);

	return true;
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
	//	requestStateClear();
	//	requestStackPush(States::Menu);
		requestStackPush(States::Pause);
	}

	if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
	{
		sf::Vector2i mousePosition = sf::Mouse::getPosition(nWindow);

		/* set view again for window before getting coords */
		nWindow.setView(*nWorldView);
		sf::Vector2f worldPosition = nWindow.mapPixelToCoords(mousePosition);

//		std::cout << "Coord X: " << worldPosition.x << std::endl;
//		std::cout << "Coord Y: " << worldPosition.y << std::endl;
	}

	// mouse zoom in and zoom out
	if (event.type == sf::Event::MouseWheelMoved)
	{
		if (event.mouseWheel.delta > 0)
			nWorldView->zoom(nZoomSpeed);
		else if (event.mouseWheel.delta < 0)
			nWorldView->zoom(1 / nZoomSpeed);
	}


//	nGUIContainer.handleEvent(event);

	return true;
}
