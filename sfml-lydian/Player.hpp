#ifndef PLAYER_HPP
#define PLAYER_HPP

// player class to handle all ioputs/events

#include <SFML/Window/Event.hpp>		// sf::Event

#include "Command.hpp"

#include <map>

// forward declare CommandQueue class
class CommandQueue;


class Player
{
public:
	// enum to represent action associated with movements, for assigning keys
	enum Action
	{
		MoveLeft,
		MoveUp,
		MoveRight,
		MoveDown,
		LaunchNormal,
		LaunchSpecial,

		AttackRight,
		AttackUpRight,
		AttackUp,
		AttackUpLeft,
		AttackLeft,
		AttackDownLeft,
		AttackDown,
		AttackDownRight,

		ChangeProjectile,

		ActionCount
	};

	enum MissionStatus
	{
		MissionRunning,	
		MissionSuccess,
		MissionFailure
	};

public:
	Player();				// keybindings are set in constructor

	/* for key bindings */
	void assignKey(Action action, sf::Keyboard::Key key);		// assign action to key

	sf::Keyboard::Key getAssignedKey(Action action) const;				// get key from action

	// hanlde on time events, pass in event and queue
	void handleEvent(const sf::Event& event, CommandQueue& commands);

	// handle realtime states, only pass in queue
	void handleRealtimeInput(CommandQueue& commands);	

	// set/get missions
	void setMissionStatus(MissionStatus status);
	MissionStatus getMissionStatus() const;

private:
	/* for action bindings */
	void initializeActions();				// action/command implementations done here


	// hardcoding actions for handleRealtime Input
	static bool isRealtimeAction(Action action);			// check if action is real time or event driven

private:
	float nPlayerSpeed;

	std::map<sf::Keyboard::Key, Action> nKeyBinding;		// key to action map
	std::map<Action, Command> nActionBinding;				// action to command map


	MissionStatus nCurrentMissionStatus;
};



#endif