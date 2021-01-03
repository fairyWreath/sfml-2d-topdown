#include "Player.hpp"
#include "Character.hpp"
#include "CommandQueue.hpp"

using namespace std::placeholders;		// for std::bind 

#include <iostream>

// constructor where all keybindings are set
Player::Player(sf::RenderWindow& window) :
	nWindow(window),
	nCurrentMissionStatus(MissionRunning),
	nWorldView(nullptr)
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

	nMouseBinding[sf::Mouse::Button::Right] = MoveToLocation;

	initializeActions();

	// set all commands in cations to character category
	for (auto& pair : nKeyboardActionBinding)
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
		if (sf::Keyboard::isKeyPressed(pair.first) && isRealtimeKeyboardAction(pair.second))
		{
			// push to command queue if match
			commands.push(nKeyboardActionBinding[pair.second]);			// python like access to std::map
		}
	}
}

struct DirectedEntityMover
{
	DirectedEntityMover(sf::Vector2f destination) : destination(destination) { }

	void operator() (Entity& entity, sf::Time) const
	{
		MovementComponent* component = entity.getMovementComponent();
		component->moveToLocation(destination.x, destination.y);
	}

	sf::Vector2f destination;
};


void Player::setWorldView(sf::View& view)
{
	nWorldView = &view;
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
		if (found != nKeyBinding.end() && !isRealtimeKeyboardAction(found->second))
			commands.push(nKeyboardActionBinding[found->second]);
	}

	// for mouse events
	if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Right)
	{
		assert(nWorldView != nullptr);

		sf::Vector2i mousePosition = sf::Mouse::getPosition(nWindow);
		
		/* set view again for window before getting coords */
		nWindow.setView(*nWorldView);
		sf::Vector2f worldPosition = nWindow.mapPixelToCoords(mousePosition);

		Command command;
		command.category = Category::PlayerCharacter;
		command.action = derivedAction<Entity>(DirectedEntityMover(worldPosition));
		commands.push(command);
	}
}


// bind key to action
void Player::assignKey(KeyboardAction action, sf::Keyboard::Key key)
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
sf::Keyboard::Key Player::getAssignedKey(KeyboardAction action) const
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


// functor
struct EntityMover
{
	EntityMover(MovementComponent::Direction direction) : direction(direction) { }

	void operator() (Entity& character, sf::Time) const
	{
		float speed = character.getEntitySpeed();
	//	float speed = 200.f;
		MovementComponent* component = character.getMovementComponent();
		component->moveToDirection(speed, direction);
	}

	MovementComponent::Direction direction;
};


// adding commands/action bindings
void Player::initializeActions()
{
	//nActionBinding[MoveLeft].action = derivedAction<Character>(
	//	std::bind([&](Character& character)
	//		{
	//			float speed = character.getCharacterSpeed();
	//			MovementComponent* component = character.getMovementComponent();
	//			component->moveToDirection(speed, MovementComponent::Direction::Left);
	//		}, _1));


	nKeyboardActionBinding[MoveLeft].action = derivedAction<Entity>(EntityMover(MovementComponent::Left));
	nKeyboardActionBinding[MoveUp].action = derivedAction<Entity>(EntityMover(MovementComponent::Up));
	nKeyboardActionBinding[MoveRight].action = derivedAction<Entity>(EntityMover(MovementComponent::Right));
	nKeyboardActionBinding[MoveDown].action = derivedAction<Entity>(EntityMover(MovementComponent::Down));

	// launching attacks
	// action is launchnormal from character class
	/* std::bind
	-> binds fixed parameters to function, with _1, _2 as prams/placeholder
	*/
	/*nKeyboardActionBinding[LaunchNormal].action = derivedAction<Character>(std::bind(&Character::launchNormalWithType, _1,
		Character::AttackType::NormalCircular));
	nKeyboardActionBinding[LaunchSpecial].action = derivedAction<Character>(std::bind(&Character::launchSpecial, _1));

	nKeyboardActionBinding[AttackRight].action = derivedAction<Character>(std::bind(&Character::launchNormalWithType, _1,
		Character::AttackType::SingleRight));
	nKeyboardActionBinding[AttackUpRight].action = derivedAction<Character>(std::bind(&Character::launchNormalWithType, _1,
		Character::AttackType::SingleUpRight));
	nKeyboardActionBinding[AttackUp].action = derivedAction<Character>(std::bind(&Character::launchNormalWithType, _1,
		Character::AttackType::SingleUp));
	nKeyboardActionBinding[AttackUpLeft].action = derivedAction<Character>(std::bind(&Character::launchNormalWithType, _1,
		Character::AttackType::SingleUpLeft));
	nKeyboardActionBinding[AttackLeft].action = derivedAction<Character>(std::bind(&Character::launchNormalWithType, _1,
		Character::AttackType::SingleLeft));
	nKeyboardActionBinding[AttackDownLeft].action = derivedAction<Character>(std::bind(&Character::launchNormalWithType, _1,
		Character::AttackType::SingleDownLeft));
	nKeyboardActionBinding[AttackDown].action = derivedAction<Character>(std::bind(&Character::launchNormalWithType, _1,
		Character::AttackType::SingleDown));
	nKeyboardActionBinding[AttackDownRight].action = derivedAction<Character>(std::bind(&Character::launchNormalWithType, _1,
		Character::AttackType::SingleDownRight));


	nKeyboardActionBinding[ChangeProjectile].action = derivedAction<Character>(std::bind(&Character::changeProjectile, _1));

	nKeyboardActionBinding[ModifyPlayerSpeed].action = derivedAction<Character>(std::bind(&Character::modifyCharacterSpeed, _1,
		100.f));

	nKeyboardActionBinding[BoostPlayerCharacter].action = derivedAction<Character>(std::bind(&Character::boostCharacter, _1));*/
}


// get info on action for realtime or event
bool Player::isRealtimeKeyboardAction(KeyboardAction action)
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

