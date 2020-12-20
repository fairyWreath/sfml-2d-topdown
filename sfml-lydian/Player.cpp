#include "Player.hpp"
#include "Character.hpp"
#include "CommandQueue.hpp"

using namespace std::placeholders;		// for std::bind 

#include <iostream>

// struct with functions/ command to apply
struct CharacterMover
{
	// struct constructor, set velocity in constructor
	CharacterMover(float vx, float vy)
		: velocity(vx, vy)			
	{

	}

	// overload () operator/functor, pass in SceneNode type, eg.Character class to apply changes
	// can use this struct as a FUNCTION/FUNCTOR
	void operator() (Character& character, sf::Time) const
	{	
		// a derived function exists to downcast function command to Character type
		// apply changes to passed in character
		character.setVelocity(character.getVelocity() + velocity);			// accelerate character
	}


	sf::Vector2f velocity;
};


// constructor where all keybindings are set
Player::Player()
{
	// set initial key bindings
	nKeyBinding[sf::Keyboard::A] = MoveLeft;
	nKeyBinding[sf::Keyboard::W] = MoveUp;
	nKeyBinding[sf::Keyboard::D] = MoveRight;
	nKeyBinding[sf::Keyboard::S] = MoveDown;

	nKeyBinding[sf::Keyboard::Space] = LaunchNormal;
	nKeyBinding[sf::Keyboard::B] = LaunchSpecial;

	// map action to commands
	initializeActions();

	// set all commands in cations to character category
	for (auto& pair : nActionBinding)
	{
		pair.second.category = Category::PlayerCharacter;
	}

}


// handle realtime input, function is called every frame
void Player::handleRealtimeInput(CommandQueue& commands)
{
	// iteratate through keybindings, check if they are being pressed and is a realtime input
	for (const auto& pair : nKeyBinding)		// iterate through map
	{
		if (sf::Keyboard::isKeyPressed(pair.first) && isRealtimeAction(pair.second))
		{
			// push to command queue if match
			commands.push(nActionBinding[pair.second]);			// python like access to std::map

			//std::cout << "Key detected, command pushed";
		}
	}
}


// handle one time events
void Player::handleEvent(const sf::Event& event, CommandQueue& commands)
{
	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::P)	// if key is pressed
	{
		// construct command here
		Command output;
		output.category = Category::PlayerCharacter;
		
		// lambda expression to construct action
		output.action = [](SceneNode& s, sf::Time)
		{
			std::cout << s.getPosition().x << ", " << s.getPosition().y << std::endl;	// output position
		};
		commands.push(output);			// push to command queue
	}
}


// bind key to action
void Player::assignKey(Action action, sf::Keyboard::Key key)
{
	// remove keys that are already mapped to that action
	for (auto itr = nKeyBinding.begin(); itr != nKeyBinding.end();)
	{
		// if action is the same, remove from the binding
		if (itr->second == action)
		{
			nKeyBinding.erase(itr);
			break;
		}
		else
			itr++;
	}

	// insert new binding
	nKeyBinding[key] = action;
}

// get key from action
sf::Keyboard::Key Player::getAssignedKey(Action action) const
{
	// search in keybinding
	for (const auto& pair : nKeyBinding)
	{
		if (pair.second == action)			// if match, return
			return pair.first;
	}

	// return unknown if not found
	return sf::Keyboard::Unknown;
}


// adding commands/action bindings
void Player::initializeActions()
{
	const float playerSpeed = 200.f;			// set speed

	// add commands here, add to actionbinding dict/map
	// use functors and command struct template to add commands
	nActionBinding[MoveLeft].action = derivedAction<Character>(CharacterMover(-playerSpeed, 0.f));
	nActionBinding[MoveUp].action = derivedAction<Character>(CharacterMover(0.f, -playerSpeed));
	nActionBinding[MoveRight].action = derivedAction<Character>(CharacterMover(+playerSpeed, 0.f));
	nActionBinding[MoveDown].action = derivedAction<Character>(CharacterMover(0.f, +playerSpeed));

	// launching attacks
	// action is launchnormal from character class
	/* std::bind
	-> binds fixed parameters to function, with _1, _2 as prams/placeholder
	*/
	nActionBinding[LaunchNormal].action = derivedAction<Character>(std::bind(&Character::launchNormal, _1));
	nActionBinding[LaunchSpecial].action = derivedAction<Character>(std::bind(&Character::launchSpecial, _1));
}


// get info on action for realtime or event
bool Player::isRealtimeAction(Action action)
{
	switch (action)
	{
	case MoveLeft:
	case MoveUp:
	case MoveRight:
	case MoveDown:
	case LaunchNormal:
		return true;
	default:
		return false;
	}
}