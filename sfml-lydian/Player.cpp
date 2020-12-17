#include "Player.hpp"
#include "Character.hpp"
#include "CommandQueue.hpp"

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


// handle realtime input
void Player::handleRealtimeInput(CommandQueue& commands)
{
	const float playerSpeed = 30.f;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))		// if A is pressed
	{
		// construct command here
		Command moveLeft;
		moveLeft.category = Category::PlayerCharacter;	// set category

		/* set action 
		1. pass in Character GameObject type
		2. pass in functor characterMover as function parameter
		*/
		moveLeft.action = derivedAction<Character>(CharacterMover(-playerSpeed, 0.f));			// - x to move left
		
		// push to comandQueue
		commands.push(moveLeft);
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