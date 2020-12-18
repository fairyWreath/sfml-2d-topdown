#include "Application.hpp"
#include "State.hpp"
#include "StateIdentifiers.hpp"
#include "StringHelpers.hpp"


// states
#include "TitleState.hpp"
#include "GameState.hpp"
#include "MenuState.hpp"
#include "PauseState.hpp"

// set time per frame
const sf::Time Application::TimePerFrame = sf::seconds(1.f / 60.f);

// application class constructor, initialize members
Application::Application() :
	nWindow(sf::VideoMode(640, 480), "Lydian", sf::Style::Close),		// sf::Style::Close means window has x button to close
	nTextures(),
	nFonts(),
	nPlayer(),
	nStateStack(State::Context(nWindow, nTextures, nFonts, nPlayer)),		// create new state context here and pass it in
	nStatisticsText(),
	nStatisticsUpdateTime(),
	nStatisticsNumFrames(0)
{
	// set repeateKeyEnabled of window class to false
	nWindow.setKeyRepeatEnabled(false);		// set to false means only a SINGLE event will be received

	// load fonts and textures
	nTextures.load(Textures::TitleScreen, "Media/Textures/Title-Screen.jpg");
	nTextures.load(Textures::PauseScreen, "Media/Textures/Pause-Screen.jpg");
	nFonts.load(Fonts::Main, "Media/Fonts/Overlock-Mod.ttf");

	// set misc statistics text
	nStatisticsText.setFont(nFonts.get(Fonts::Main));
	nStatisticsText.setFillColor(sf::Color::Magenta);
	nStatisticsText.setPosition(5.f, 5.f);			// set top right corner
	nStatisticsText.setCharacterSize(15);			// set char size to 10

	// register states 
	registerStates();

	// push title state to stack
	nStateStack.pushState(States::Title);
	
}

// main function to run game
void Application::run()
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
			processInput();

			// update scene graph
			update(TimePerFrame);

			// if statestack is empty, close window
			if (nStateStack.isEmpty())
				nWindow.close();
		}

		// update statstics here 
		updateStatistics(elapsedTime);

		render();			// render changes
	}

}


// process game events/input handling
// use state stack to handle the events
void Application::processInput()
{
	sf::Event event;
	while (nWindow.pollEvent(event))
	{
		// let statestack handle all events of the states
		nStateStack.handleEvent(event);

		// if window x button is pressed
		if (event.type == sf::Event::Closed)
			nWindow.close();
	}
}



// function for updates
// again, let the state stack handle everything
void Application::update(sf::Time elapsedTime)
{
	nStateStack.update(elapsedTime);
}


// function for rendering
void Application::render()
{
	nWindow.clear();		// clear window
	
	// draw from statestack
	nStateStack.draw();

	nWindow.setView(nWindow.getDefaultView());			// get 1 1 viewport

	// draw fonts here, etc.
	nWindow.draw(nStatisticsText);

	nWindow.display();			// display scene graph, apply changes
}


// update misc/statistics text
void Application::updateStatistics(sf::Time elapsedTime)
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


// register the states to the state stack
void Application::registerStates()
{
	// register all of the states, enum in () as the key to the map
	nStateStack.registerState<TitleState>(States::Title);		// <TitleState> is the template here
	nStateStack.registerState<GameState>(States::Game);			
	nStateStack.registerState<MenuState>(States::Menu);
	nStateStack.registerState<PauseState>(States::Pause);
}

