#include "Player.hpp"
#include "Character.hpp"
#include "CommandQueue.hpp"

using namespace std::placeholders;		// for std::bind 

#include <iostream>


struct CharacterMover
{
	enum Direction
	{
		Right,
		Up,
		Left,
		Down
	};

	// struct constructor, set velocity in constructor
	CharacterMover(Direction direction) :
		direction(direction)
	{

	}

	// overload () operator/functor, pass in SceneNode type, eg.Character class to apply changes
	// can use this struct as a FUNCTION/FUNCTOR
	void operator() (Character& character, sf::Time) const
	{
		// a derived function exists to downcast function command to Character type
		// apply changes to passed in character
		sf::Vector2f velocity;
		float speed = character.getCharacterSpeed();

		switch(direction)
		{
		case Right:
			velocity = sf::Vector2f(+speed, 0.f);
			break;
		case Up:
			velocity = sf::Vector2f(0.f, -speed);
			break;
		case Left:
			velocity = sf::Vector2f(-speed, 0.f);
			break;
		case Down:
			velocity = sf::Vector2f(0.f, +speed);
			break;
		}

		character.setVelocity(character.getVelocity() + velocity);			// accelerate character
	}


	Direction direction;
};

// constructor where all keybindings are set
Player::Player() :
	nCurrentMissionStatus(MissionRunning)
{
	// set initial key bindings
	nKeyBinding[sf::Keyboard::Left] = MoveLeft;
	nKeyBinding[sf::Keyboard::Up] = MoveUp;
	nKeyBinding[sf::Keyboard::Right] = MoveRight;
	nKeyBinding[sf::Keyboard::Down] = MoveDown;

	nKeyBinding[sf::Keyboard::Space] = LaunchNormal;
	nKeyBinding[sf::Keyboard::Num1] = LaunchSpecial;

	nKeyBinding[sf::Keyboard::D] = AttackRight;
	nKeyBinding[sf::Keyboard::E] = AttackUpRight;
	nKeyBinding[sf::Keyboard::W] = AttackUp;
	nKeyBinding[sf::Keyboard::Q] = AttackUpLeft;
	nKeyBinding[sf::Keyboard::A] = AttackLeft;
	nKeyBinding[sf::Keyboard::Z] = AttackDownLeft;
	nKeyBinding[sf::Keyboard::X] = AttackDown;
	nKeyBinding[sf::Keyboard::C] = AttackDownRight;

	nKeyBinding[sf::Keyboard::Num2] = ChangeProjectile;
	nKeyBinding[sf::Keyboard::Num3] = ModifyPlayerSpeed;
	nKeyBinding[sf::Keyboard::Num4] = BoostPlayerCharacter;

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
		}
	}
}


// handle one time events
void Player::handleEvent(const sf::Event& event, CommandQueue& commands)
{
	// handle one time events
	if (event.type == sf::Event::KeyPressed)
	{
		// find from keybindings
		auto found = nKeyBinding.find(event.key.code);

		// if found and action is event-based
		if (found != nKeyBinding.end() && !isRealtimeAction(found->second))
			commands.push(nActionBinding[found->second]);
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
	// add commands here, add to actionbinding dict/map
	// use functors and command struct template to add commands
	nActionBinding[MoveLeft].action = derivedAction<Character>(CharacterMover(CharacterMover::Left));
	nActionBinding[MoveUp].action = derivedAction<Character>(CharacterMover(CharacterMover::Up));
	nActionBinding[MoveRight].action = derivedAction<Character>(CharacterMover(CharacterMover::Right));
	nActionBinding[MoveDown].action = derivedAction<Character>(CharacterMover(CharacterMover::Down));

	// launching attacks
	// action is launchnormal from character class
	/* std::bind
	-> binds fixed parameters to function, with _1, _2 as prams/placeholder
	*/
	nActionBinding[LaunchNormal].action = derivedAction<Character>(std::bind(&Character::launchNormalWithType, _1,
		Character::AttackType::NormalCircular));
	nActionBinding[LaunchSpecial].action = derivedAction<Character>(std::bind(&Character::launchSpecial, _1));

	nActionBinding[AttackRight].action = derivedAction<Character>(std::bind(&Character::launchNormalWithType, _1, 
		Character::AttackType::SingleRight));
	nActionBinding[AttackUpRight].action = derivedAction<Character>(std::bind(&Character::launchNormalWithType, _1,
		Character::AttackType::SingleUpRight));
	nActionBinding[AttackUp].action = derivedAction<Character>(std::bind(&Character::launchNormalWithType, _1,
		Character::AttackType::SingleUp));
	nActionBinding[AttackUpLeft].action = derivedAction<Character>(std::bind(&Character::launchNormalWithType, _1,
		Character::AttackType::SingleUpLeft));
	nActionBinding[AttackLeft].action = derivedAction<Character>(std::bind(&Character::launchNormalWithType, _1,
		Character::AttackType::SingleLeft));
	nActionBinding[AttackDownLeft].action = derivedAction<Character>(std::bind(&Character::launchNormalWithType, _1,
		Character::AttackType::SingleDownLeft));
	nActionBinding[AttackDown].action = derivedAction<Character>(std::bind(&Character::launchNormalWithType, _1,
		Character::AttackType::SingleDown));
	nActionBinding[AttackDownRight].action = derivedAction<Character>(std::bind(&Character::launchNormalWithType, _1,
		Character::AttackType::SingleDownRight));


	nActionBinding[ChangeProjectile].action = derivedAction<Character>(std::bind(&Character::changeProjectile, _1));

	nActionBinding[ModifyPlayerSpeed].action = derivedAction<Character>(std::bind(&Character::modifyCharacterSpeed, _1,
		100.f));

	nActionBinding[BoostPlayerCharacter].action = derivedAction<Character>(std::bind(&Character::boostCharacter, _1));
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
	case AttackRight:
	case AttackUpRight:
	case AttackUp:
	case AttackUpLeft:
	case AttackLeft:
	case AttackDownLeft:
	case AttackDown:
	case AttackDownRight:
	case ChangeProjectile:
	case ModifyPlayerSpeed:
	case BoostPlayerCharacter:
		return true;
	case LaunchSpecial:
		return false;
	default:
		return false;
	}
}

// missions
void Player::setMissionStatus(MissionStatus status)
{
	nCurrentMissionStatus = status;
}

Player::MissionStatus Player::getMissionStatus() const
{
	return nCurrentMissionStatus;
}

