#ifndef STATE_HPP
#define STATE_HPP

// base class for states

#include "ResourceIdentifiers.hpp"
#include "StateIdentifiers.hpp"

#include <SFML/System/Time.hpp>
#include <SFML/Window/Event.hpp>

#include <memory>


// forward declaration of classes
namespace sf
{
	class RenderWindow;
}

class StateStack;
class Player;

class State
{
public:
	typedef std::unique_ptr<State> Ptr;			// typedef for state unique ptr
	struct Context							// state context
	{
	
	};

public:
	// constructor that accepts the STATESTACK with the context
	State(StateStack& stack, Context context);

	// virtual destructor
	virtual ~State();

	// virtual screen displaying functions
	virtual void draw() = 0;			// set to 'null'
	virtual bool update(sf::Time dt) = 0;
	virtual bool handleEvent(const sf::Event& event) = 0;

protected:
	// functions for stack operations
	void requestStackPush(States::ID stateID);
	void requestStackPop();
	void requestStateClear();

	// get state context
	Context getContext() const;

private:
	StateStack* nStack;			// pointer to state stack
	Context nContext;			// state context

};



#endif