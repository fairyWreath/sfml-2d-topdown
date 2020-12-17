#ifndef PLAYER_HPP
#define PLAYER_HPP

// player class to handle all ioputs/events

#include <SFML/Window/Event.hpp>		// sf::Event

#include "Command.hpp"

// forward declare CommandQueue class
class CommandQueue;


class Player
{
public:
	// hanlde on time events, pass in event and queue
	void handleEvent(const sf::Event& event, CommandQueue& commands);

	// handle realtime states, only pass in queue
	void handleRealtimeInput(CommandQueue& commands);			
};



#endif