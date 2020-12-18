#ifndef STATESTACK_HPP
#define STATESTACK_HPP

// stack class to manage screens for FSM

#include <SFML/System/NonCopyable.hpp>
#include <SFML/System/Time.hpp>

#include "State.hpp"
#include "StateIdentifiers.hpp"

#include <vector>
#include <functional>
#include <map>

// forward declare sf classes
namespace sf
{
	class Event;
	class RenderWindow;
}

class StateStack : private sf::NonCopyable
{
public:
	enum Action		// stack actions in enums
	{
		Push,
		Pop,
		Clear
	};


public:
	explicit StateStack(State::Context context);			// pass in state context to constructor

	// member function to insert factory mappings
	template <typename T>
	void registerState(States::ID stateID);

	/* drawing/updating the screen
	-> go throught the stack, from highest(top) to lowest and update
	*/
	void update(sf::Time dt);
	void draw();


	// handle input events, from highest to lowest in the state stack
	void handleEvent(const sf::Event& event);

	// stack operations, to be inserted into the PENDING LIST, executed at the end of an iteration only when it is safe
	// to do so
	void pushState(States::ID stateID);
	void popState();
	void clearStates();

	bool isEmpty() const;				// check state of the statestack

private:
	
	/* create new state from states id, returns a unique_ptr of the newly created state
	-> access from the nFactories that have been registered
	*/
	State::Ptr createState(States::ID stateID);

	// apply active stack changes at the END, after itearing in update/handle events
	// register changes to a pending action list where they can be proccessed when it is safe, after the iteration
	void applyPendingChanges();

private:
	// struct class for pending changes, stack operations to be done AFTEr iteration
	// actual stack operations are done through here
	struct PendingChange
	{
		explicit PendingChange(Action action, States::ID stateID = States::None);			// set default to none if not given
		Action action;			// enum for stack operation type
		States::ID stateID;
	};


private:
	// the main stack containing the ACTIVE STATES
	std::vector<State::Ptr> nStack;				// stack placed inside std::vector
	
	// stack for pending changes, to be applied after an iteration
	std::vector<PendingChange> nPendingList;			// vector of changes to be made

	State::Context nContext;			// state context

	// std::map with factory functions where changes can be made
	// factory functions that create a new state ON DEMAND
	std::map<States::ID, std::function<State::Ptr()>> nFactories;			// function return Ptr() type

};

// templates are defined in headers
// insert mappings to factory functions for calling state on-demand
template <typename T>
void StateStack::registerState(States::ID stateID)
{
	/* lambda expression for map value
	*/
	nFactories[stateID] = [this]()				// this capture clause to get statestack
	{
		return State::Ptr(new T(*this, nContext));			// create new statepointer, cnstrcutro passed in current statestack and context
	};
}

#endif