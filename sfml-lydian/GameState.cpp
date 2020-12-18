#include "GameState.hpp"

GameState::GameState(StateStack& stack, Context context) :
	State(stack, context),			// constructor for PARENT class
	// shared values are stored in the Context structure
	nWorld(*context.window),		// initalize world class with context window
	nPlayer(*context.player)
{
}

// draw for GameState means drawing the world and all of its scenes
void GameState::draw()
{
	nWorld.draw();
}

// these functions mimic our original game class

// update scenes
bool GameState::update(sf::Time dt)
{
	// update the world
	nWorld.update(dt);

	// get command queue and handle inputs here, for REAL TIME INPUTS, let player class handle
	CommandQueue& commands = nWorld.getCommandQueue();
	nPlayer.handleRealtimeInput(commands);

	return true;
}

// handle events
bool GameState::handleEvent(const sf::Event& event)
{
	// handle event based inputs, let player class handle
	CommandQueue& commands = nWorld.getCommandQueue();
	nPlayer.handleEvent(event, commands);

	/* requestStackPush is used here to NAVIGATE BETWEEN STATES
	-> if esc is pressed, order from this state to push the pause state to the stack
	*/
	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
		requestStackPush(States::Pause);			// push pause state

	return true;
}



