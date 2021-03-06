#ifndef MENUSTATE_HPP
#define MENUSTATE_HPP


#include "State.hpp"
#include "Container.hpp"

#include <SFML/Graphics/Sprite.hpp>

class MenuState : public State
{
public:
	// constructor, pass in State class parameters
	MenuState(StateStack& stack, Context context);

	// virtual function for State class to be overidden
	virtual void draw();
	virtual bool update(sf::Time dt);
	virtual bool handleEvent(const sf::Event& event);

private:
	// sfml members
	sf::Sprite nBackgroundSprite;

	// GUI container
	GUI::Container nGUIContainer;


};


#endif