#ifndef GAMEENDSTATE_HPP
#define GAMEENDSTATE_HPP

#include "State.hpp"

#include "Container.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Time.hpp>

class GameEndState : public State
{
public:
	GameEndState(StateStack& stack, Context context);

	// virtual function for State class to be overidden
	virtual void draw();
	virtual bool update(sf::Time dt);
	virtual bool handleEvent(const sf::Event& event);

private:
	// sfml members
	sf::Sprite nBackgroundSprite;
	sf::Time nElapsedTime;

	GUI::Container nGUIContainer;
};


#endif