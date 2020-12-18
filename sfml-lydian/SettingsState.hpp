#ifndef SETTINGSSTATE_HPP
#define SETTINGSSTATE_HPP

// state for settings / keybindings

#include "State.hpp"
#include "Player.hpp"
#include "Container.hpp"
#include "Button.hpp"
#include "Label.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

#include <array>		// std::array, fixed sized stl array

class SettingsState : public State
{
public:
	// constructor with params for State class
	SettingsState(StateStack& stack, Context context);

	// virutal functions of State to override
	virtual void draw();
	virtual bool update(sf::Time dt);
	virtual bool handleEvent(const sf::Event& event);

private:
	// update stting labels
	void updateLabels();

	// add button, with parameters for text and player action(keybindings), y for position
	void addButtonLabel(Player::Action action, float y, const std::string& text, Context context);

private:
	sf::Sprite nBackgroundSprite;
	GUI::Container nGUIContainer;

	// fixed arrays for label and bindings, based on Player class action count
	// std::array<typename ,size>
	std::array<GUI::Button::Ptr, Player::ActionCount> nBindingButtons;
	std::array<GUI::Label::Ptr, Player::ActionCount> nBindingLabels;

};


#endif