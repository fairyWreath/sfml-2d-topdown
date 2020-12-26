#include "State.hpp"
#include "StateStack.hpp"

// constructor for Context structure
State::Context::Context(sf::RenderWindow& window, TextureHolder& textures, FontHolder& fonts, Player& player, 
	MusicPlayer& music, SoundPlayer& sound) :
	window(&window),		// pass in ptr
	textures(&textures),
	fonts(&fonts),
	player(&player),
	musicPlayer(&music),
	soundPlayer(&sound)
{
}

// constructor for the state class itself
State::State(StateStack& stack, Context context) :
	nStack(&stack),		// set statestack pointer
	nContext(context)		// initialize context
{
}

// virtual destructor
State::~State()
{
	// do nothing here
}

// request operations to handle the stack from curren state
void State::requestStackPush(States::ID stateID)
{
	nStack->pushState(stateID);
}

void State::requestStackPop()
{
	nStack->popState();
}

void State::requestStateClear()
{
	nStack->clearStates();
}

// get context from current state
State::Context State::getContext() const
{
	return nContext;
}


