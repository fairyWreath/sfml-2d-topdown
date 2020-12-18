#include "StateStack.hpp"

#include <cassert>


// constructor for statestack
StateStack::StateStack(State::Context context) :
	nStack(),				// set stack, pending list, factories to empty
	nPendingList(),
	nContext(context),
	nFactories()
{

}


// create new State from nFactories map, reutrning a unique_ptr to that state
State::Ptr StateStack::createState(States::ID stateID)
{
	auto found = nFactories.find(stateID);
	assert(found != nFactories.end());			// check with assertion

	return found->second();					// run the std::function of the map and return that unique_ptr
}


// every poll event is fed to the state stack, and the stack will deliver the event to the active states
void StateStack::handleEvent(const sf::Event& event)
{
	// use REVERSE ITERATOR to mimic a stack
	for (auto itr = nStack.rbegin(); itr != nStack.rend(); itr++)
	{
		// let state in the stack handle the event
		// if false is detected immediately end the loop
		if (!(*itr)->handleEvent(event))		
			return;
	}

	// apply stack operations after stack iteration above, when it is already safe
	applyPendingChanges();
}


// udpate states, from top stack to lowest
void StateStack::update(sf::Time dt)
{
	// iterate from top to bottom, stop when false is obtained
	for (auto itr = nStack.rbegin(); itr != nStack.rend(); itr++)
	{
		if (!(*itr)->update(dt))		// call update function of the state
			return;
	}
	// apply stack operations after stack iteration above, when it is already safe
	applyPendingChanges();
}

// draw states, from top stack to lowest
void StateStack::draw()
{
	// draw all active states, from top to bottom
	for (const auto& state : nStack)
	{
		state->draw();
	}
}


// apply pending stack operations, after iterations or when it is safe to do so
void StateStack::applyPendingChanges()
{
	// go through pending list
	for (const PendingChange& change : nPendingList)
	{
		// match enum type
		switch (change.action)
		{
		case Push:
			nStack.push_back(createState(change.stateID));			// create new state based on change struct state id
			break;
	
		case Pop:
			nStack.pop_back();			// pop stack from back
			break;

		case Clear:
			nStack.clear();
			break;
		}
	}

	// clear pending list
	nPendingList.clear();
}


// pendingchange type constructor
StateStack::PendingChange::PendingChange(Action action, States::ID stateID) :
	action(action),
	stateID(stateID)
{

}


// stack operations, goes into the pendingList
void StateStack::pushState(States::ID stateID)
{
	nPendingList.push_back(PendingChange(Push, stateID));		// create new change type and push it
}

void StateStack::popState()
{
	nPendingList.push_back(PendingChange(Pop));				// constructor for states::ID overloaded, passed in none stateid
}

void StateStack::clearStates()
{
	nPendingList.push_back(PendingChange(Clear));		// push clear Action
}

bool StateStack::isEmpty() const
{
	return nStack.empty();
}
