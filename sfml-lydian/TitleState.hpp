#ifndef TITLESTATE_HPP
#define TITLESTATE_HPP

// class for title state or title screen

#include "State.hpp"
#include "Container.hpp"

// class will contain sprite and text
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>


// inherit from state class
class TitleState : public State
{
public:
	// constructor, pass in to State constructor
	TitleState(StateStack& stack, Context context);

	// virtual functions for drawing, updating and handling events
	virtual void draw();
	virtual bool update(sf::Time dt);
	virtual bool handleEvent(const sf::Event& event);

private:
	// sfml member classes to store sprites, etc. for menu
	sf::Sprite nBackgroundSprite;		// background sprite for title menu
	
	sf::Text nText;
	bool nShowText;					// used for blinking
	sf::Time nTextEffectTime;

	GUI::Container nGUIContainer;		// for title label
};


#endif