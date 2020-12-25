#ifndef COMMANDQUEUE_HPP
#define COMMANDQUEUE_HPP

// class to transport commands to the world and the scene graph


#include <queue>			// std::queue

#include "Command.hpp"

class CommandQueue
{
public:
	// queue methods
	void push(const Command& command);
	Command pop();

	// check state
	bool isEmpty() const;
	std::queue<Command> nQueue;
private:
	
};


#endif