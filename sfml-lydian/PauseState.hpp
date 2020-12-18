#ifndef PAUSESTATE_HPP
#define PAUSESTATE_HPP

// class for pause state

#include "State.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>


class PauseState : public State
{
public:
	// constructor, pass in State class parameters
	PauseState(StateStack& stack, Context context);

	// virtual function for State class to be overidden
	virtual void draw();
	virtual bool update(sf::Time dt);
	virtual bool handleEvent(const sf::Event& event);



private:
	// sfml members
	sf::Sprite nBackgroundSprite;
	sf::Text nPausedText;
	sf::Text nInstructionText;

};

#endif