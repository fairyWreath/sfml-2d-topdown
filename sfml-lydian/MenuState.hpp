#ifndef MENUSTATE_HPP
#define MENUSTATE_HPP

// class for menu state

#include "State.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>


class MenuState : public State
{
public:
	// constructor, pass in State class parameters
	MenuState(StateStack& stack, Context context);

	// virtual function for State class to be overidden
	virtual void draw();
	virtual bool update(sf::Time dt);
	virtual bool handleEvent(const sf::Event& event);
	

	// for displaying/updating the option text
	void updateOptionText();

private:
	// enum for options/state navigation
	enum OptionNames
	{
		Play, 
		Exit
	};

private:
	// sfml members
	sf::Sprite nBackgroundSprite;

	// vector of text coressponding to the enum
	std::vector<sf::Text> nOptions;
	std::size_t nOptionIndex;

};


#endif