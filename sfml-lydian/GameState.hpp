#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP

// class for game state

#include "State.hpp"
#include "World.hpp"
#include "Player.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>


class GameState : public State
{
public:
	GameState(StateStack& stack, Context context);			// parameters of State

	virtual void draw();
	virtual bool update(sf::Time dt);
	virtual bool handleEvent(const sf::Event& event);

private:
	// world class
//	World nWorld;
	World* nWorld;

	// memadress to palyer
	Player& nPlayer;

};


#endif