#include "SettingsState.hpp"
#include "Utility.hpp"
#include "ResourceHolder.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

#include <iostream>

#define SETTINGS_COUNT 6

SettingsState::SettingsState(StateStack& stack, Context context) :
	State(stack, context),
	nGUIContainer()
{
	// set background sprite
	nBackgroundSprite.setTexture(context.textures->get(Textures::SettingsScreen));

	// draw buttons and labels

	// 66.5 x to centre button on left column
	addButtonLabel(Player::MoveLeft, 40.f, 150.f, "Left", context);
	addButtonLabel(Player::MoveRight, 40.f, 230.f, "Right", context);
	addButtonLabel(Player::MoveUp, 40.f, 310.f, "Up", context);
	addButtonLabel(Player::MoveDown, 40.f, 390.f, "Down", context);

	addButtonLabel(Player::LaunchNormal, 640.f, 150.f, "Normal Attack", context);
	addButtonLabel(Player::LaunchSpecial, 640.f, 230.f, "Special Attack", context);


	updateLabels();			// update initial labels

	// create button to go back, pop settingstate off the stack
	auto backButton = std::make_shared<GUI::Button>(*context.fonts, *context.textures);

	// 386.5f x to centre button
	backButton->setPosition(386.5f, 550.f);
	backButton->setText("Back");
	// use std::bind to set std::function, pop settings state off the stack
	backButton->setCallback(std::bind(&SettingsState::requestStackPop, this));

	// settings label
	auto settingsLabel = std::make_shared<GUI::Label>("Settings", *context.fonts);
	settingsLabel->setPosition(80.f, 20.f);
	settingsLabel->setColor(sf::Color::Cyan);
	settingsLabel->setFont(context.fonts->get(Fonts::Title));		// delanova
	settingsLabel->setSize(90);


	// pack the back button
	nGUIContainer.pack(backButton);
	nGUIContainer.pack(settingsLabel);
}


// draw based on ui container
void SettingsState::draw()
{
	// get window first
	sf::RenderWindow& window = *getContext().window;		

	window.draw(nBackgroundSprite);
	window.draw(nGUIContainer);
}

// no animations to update
bool SettingsState::update(sf::Time)
{
	return true;
}


// handle key events, change bindings
bool SettingsState::handleEvent(const sf::Event& event)
{
	bool isKeyBinding = false;		// to know if keybindings are changed or not

	// iterate through binding buttons to check if a key is being pressed
	for (std::size_t action = 0; action < SETTINGS_COUNT; action++)
	{
		// first 'activate' key to change with enter
		if (nBindingButtons[action]->isActive())	// if a button is toggled/active, from component class
		{
			isKeyBinding = true;
			if (event.type == sf::Event::KeyReleased)
			{
				// assignkeybinding in player class, based on key.code
				getContext().player->assignKey(static_cast<Player::Action>(action), event.key.code);
				nBindingButtons[action]->deactivate();		// deactivate the button
			}
			break;
		}
	}


	if (isKeyBinding)
	{
		updateLabels();
	}
	else
	{
		// esc to go back, pop settings off stack
		if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
		{
			requestStackPop();
		}


		nGUIContainer.handleEvent(event);		// if no, pass event to gui components
	}

	return false;
}



// update labels based on set binding
void SettingsState::updateLabels()
{
	// get player
	Player& player = *getContext().player;

	// set all labels based on the bindings
	for (std::size_t i = 0; i < SETTINGS_COUNT; i++)
	{
		// get the key, changing i size_t type to enum Action type
		sf::Keyboard::Key key = player.getAssignedKey(static_cast<Player::Action>(i));
		
		// set key to string and change label
		nBindingLabels[i]->setText(toString(key));
	}
}


// add button and label based on given action parameter, y param for position, string param for button text
void SettingsState::addButtonLabel(Player::Action action, float x, float y, const std::string& text, Context context)
{
	// create button, in ths static array
	nBindingButtons[action] = std::make_shared<GUI::Button>(*context.fonts, *context.textures);
	nBindingButtons[action]->setPosition(x, y);
	nBindingButtons[action]->setText(text);
	nBindingButtons[action]->setToggle(true);			// button can be toggled

	// create label
	nBindingLabels[action] = std::make_shared<GUI::Label>("", *context.fonts);
	nBindingLabels[action]->setPosition(x + 520.f, y + 12.f);		// place beside button

	// pack to container
	nGUIContainer.pack(nBindingButtons[action]);
	nGUIContainer.pack(nBindingLabels[action]);
}