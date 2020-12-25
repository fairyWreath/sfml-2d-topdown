#include "CommandQueue.hpp"

// queue methods
void CommandQueue::push(const Command& command)
{
	nQueue.push(command);
}

Command CommandQueue::pop()
{
	Command front = nQueue.front();			// access first in element
	nQueue.pop();					// does not return, simply pops
	return front;
}

bool CommandQueue::isEmpty() const
{
	return nQueue.empty();
}

