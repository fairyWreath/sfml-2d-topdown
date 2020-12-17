#include "Game.hpp"


#include <iostream>			// input handling debugging

// set time per frame
const sf::Time Game::TimePerFrame = sf::seconds(1.f / 60.f);

Game::Game() :
	nWindow(sf::VideoMode(640, 480), "Lydian", sf::Style::Close),		// initialize render window
	nWorld(nWindow),		// intiialize world class
	nFont(),				// misc initializations
	nStatisticsText(),
	nStatisticsUpdateTime(),
	nStatisticsNumFrames(0),
	nIsMovingUp(false),					// set movement states to false
	nIsMovingDown(false),
	nIsMovingLeft(false),
	nIsMovingRight(false),
	nIsPaused(false)
{

	// load fonts and misc text here
	nFont.loadFromFile("Media/Fonts/Overlock-Mod.ttf");
	nStatisticsText.setFillColor(sf::Color::Magenta);
	nStatisticsText.setFont(nFont);
	nStatisticsText.setPosition(5.f, 5.f);			// set top right corner
	nStatisticsText.setCharacterSize(15);			// set char size to 10
}


// main function to run game
void Game::run()
{
	sf::Clock clock;			// start clock for frames
	sf::Time timeSinceLastUpdate = sf::Time::Zero;		// initialze lastupdate time to zero for the first time

	// main game loop
	while (nWindow.isOpen())
	{
		sf::Time elapsedTime = clock.restart();			// restart frames clock
		timeSinceLastUpdate += elapsedTime;				// add to master time

		// if time is more, apply changes
		while (timeSinceLastUpdate > TimePerFrame)
		{
			timeSinceLastUpdate -= TimePerFrame;

			// process events here
			processEvents();

			// if not paused
			if (!nIsPaused)
				update(TimePerFrame);
		}

		// update statstics here
		updateStatistics(elapsedTime);

		render();			// render changes
	}

}


// process game events/input handling
void Game::processEvents()
{
	sf::Event event;
	while (nWindow.pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::Closed:				// window x button
			nWindow.close();
			break;

		// other different event types
		
			/* key pressing */
		case sf::Event::KeyPressed:
			handlePlayerInput(event.key.code, true);
			break;

		case sf::Event::KeyReleased:
			handlePlayerInput(event.key.code, false);
			break;

			/*	  SCREEN FOCUS	  */
		case sf::Event::GainedFocus:
			nIsPaused = false;
			break;
		case sf::Event::LostFocus:			// lost focus
			nIsPaused = true;
			break;


			/*    MOUSE EVENTS    */
		case sf::Event::MouseEntered:
			// triggered when mouse enters the window
			break;
		case sf::Event::MouseLeft:
			// triggered when mouse leaves the window
			break;
		case sf::Event::MouseMoved:
			// triggered when mouse moves INSIDE the window
			// access the data from event.mouseMove member
			std::cout << "mouse moved inside window: " << event.mouseMove.x
				<< " , " << event.mouseMove.y << std::endl;

			break;
		case sf::Event::MouseButtonPressed:
			// triggered when button is pressed
			break;
		case sf::Event::MouseButtonReleased:
			// triggered when mouse button is released
			break;
		case sf::Event::MouseWheelMoved:
			// triggered when wheel is moved
			break;

		}
	}
}



// function for updates
void Game::update(sf::Time elapsedTime)
{
	// update movement based on states
	sf::Vector2f movement(0.f, 0.f);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))		// ask for real time input here without processing events
		movement.y -= 1.f;	


	// nPlayer.move(movement);			// sf::Transformable parent move method

	// update world scene graph
	nWorld.update(elapsedTime);
}


// function for rendering
void Game::render()
{
	nWindow.clear();		// clear window
	nWorld.draw();			// draw scene graph

	nWindow.setView(nWindow.getDefaultView());			// get 1 1 viewport
	
	// draw fonts here, etc.
	nWindow.draw(nStatisticsText);

	nWindow.display();			// display scene graph, apply changes
}


// update misc/statistics text
void Game::updateStatistics(sf::Time elapsedTime)
{
	nStatisticsUpdateTime += elapsedTime;		// add to total time
	nStatisticsNumFrames += 1;			// add numframes

	if (nStatisticsUpdateTime >= sf::seconds(1.0f))			// if more than 1
	{
		// dislpay frame data
		nStatisticsText.setString(
			"Frames / Second = " + toString(nStatisticsNumFrames) + "\n" +
			"Time / Update = " + toString(nStatisticsUpdateTime.asMicroseconds() / nStatisticsNumFrames) + "us");
		
		// reset statistics
		nStatisticsUpdateTime -= sf::seconds(1.0f);
		nStatisticsNumFrames = 0;
	}
}


// handle real time inputs, not from events
void Game::handlePlayerInput(sf::Keyboard::Key key, bool isPressed)
{
	if (key == sf::Keyboard::W)				// if pressed k is W
		nIsMovingUp = isPressed;			// set moveup state to true
}
