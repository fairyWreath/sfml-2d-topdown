#ifndef PLAYER_HPP
#define PLAYER_HPP

// player input component

#include <SFML/Window/Event.hpp>		// sf::Event
#include <SFML/Graphics/RenderWindow.hpp>


#include "Command.hpp"

#include <map>

// forward declare CommandQueue class
class CommandQueue;


class Player
{
public:
	// enum to represent action associated with movements, for assigning keys
	enum KeyboardAction
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
		ModifyPlayerSpeed,
		BoostPlayerCharacter,

		KeyboardActionCount
	};

	enum MouseAction
	{
		MoveToLocation,

		MouseActionCount
	};

	enum MissionStatus
	{
		MissionRunning,	
		MissionSuccess,
		MissionFailure
	};

public:
	Player(sf::RenderWindow& window);				// keybindings are set in constructor

	/* for key bindings */
	void assignKey(KeyboardAction action, sf::Keyboard::Key key);		// assign action to key

	sf::Keyboard::Key getAssignedKey(KeyboardAction action) const;				// get key from action

	// hanlde on time events, pass in event and queue
	void handleEvent(const sf::Event& event, CommandQueue& commands);

	// handle realtime states, only pass in queue
	void handleRealtimeInput(CommandQueue& commands);	

	void setWorldView(sf::View& view);

	// set/get missions
	void setMissionStatus(MissionStatus status);
	MissionStatus getMissionStatus() const;

private:
	/* for action bindings */
	void initializeActions();				// action/command implementations done here


	// hardcoding actions for handleRealtime Input
	static bool isRealtimeKeyboardAction(KeyboardAction action);			// check if action is real time or event driven

private:

	std::map<sf::Keyboard::Key, KeyboardAction> nKeyBinding;		// key to action map
	std::map<KeyboardAction, Command> nKeyboardActionBinding;				// action to command map

	std::map<sf::Mouse::Button, MouseAction> nMouseBinding;
	std::map<MouseAction, Command> nMouseActionBinding;

	MissionStatus nCurrentMissionStatus;

	sf::RenderWindow& nWindow;

	sf::View* nWorldView;
};



#endif