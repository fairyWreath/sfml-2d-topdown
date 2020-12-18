#include "MenuState.hpp"
#include "ResourceHolder.hpp"
#include "Utility.hpp"
#include "Button.hpp"
#include "Label.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>


#include <iostream>		// for debugging

// MenuState constructor
MenuState::MenuState(StateStack& stack, Context context) :
	State(stack, context),
	nGUIContainer()			// intialize container
{
	// set baclground sprite, load from content
	sf::Texture& texture = context.textures->get(Textures::MenuScreen);
	nBackgroundSprite.setTexture(texture);


	 //std::cout << "Passed menuscreen\n";

	/* create buttons (or other components) here		*/

	// create button ptr, pass in context fonts and textures. the fonts/textures to be used already handled in the Button class
	GUI::Button::Ptr playButton = std::make_shared<GUI::Button>(*context.fonts, *context.textures);
	playButton->setPosition(400, 350);		// set position of the sf::Transformable here
	playButton->setText("Play");
	// use lambda function to set callback
	playButton->setCallback([this]()		// capture clause MenuState ptr, no parameters
		{
			// pop menu state and go to game
			requestStackPop();
			requestStackPush(States::Game);
		});

//	std::cout << "Passed button\n";

	// settings button
	GUI::Button::Ptr settingsButton = std::make_shared<GUI::Button>(*context.fonts, *context.textures);
	settingsButton->setPosition(400, 430);		// set position of the sf::Transformable here
	settingsButton->setText("Settings");
	// use lambda function to set callback
	settingsButton->setCallback([this]()		// capture clause MenuState ptr, no parameters
		{
			// push settings state to stack
			requestStackPush(States::Settings);
		});

	// return to title menu
	auto titleButton = std::make_shared<GUI::Button>(*context.fonts, *context.textures);
	titleButton->setPosition(400, 510);
	titleButton->setText("Return to Title");
	titleButton->setCallback([this]()
		{
			// clear and go to menu
			requestStateClear();
			requestStackPush(States::Title);
		});

	// exit button
	GUI::Button::Ptr exitButton = std::make_shared<GUI::Button>(*context.fonts, *context.textures);
	exitButton->setPosition(400, 590);
	exitButton->setText("Exit");
	exitButton->setCallback([this]() 
		{
			requestStackPop();		// pop menu state and exit game
		});


	// create title label
	auto titleLabel = std::make_shared<GUI::Label>("Easy Carefree Existence", *context.fonts);
	titleLabel->setPosition(250, 100);
	titleLabel->setSize(120);
	titleLabel->setColor(sf::Color(164, 231, 223));
	titleLabel->setFont(context.fonts->get(Fonts::Title));		// set custom font

	// pack to container
	nGUIContainer.pack(playButton);
	nGUIContainer.pack(settingsButton);
	nGUIContainer.pack(titleButton);
	nGUIContainer.pack(exitButton);

	nGUIContainer.pack(titleLabel);
}

// override virtual functions for update, draw and handling events
void MenuState::draw()
{
	// get window memaddress from context
	sf::RenderWindow& window = *getContext().window;

	window.setView(window.getDefaultView());		// set viewport to default
	window.draw(nBackgroundSprite);				// draw the sprite

	// draw container
	window.draw(nGUIContainer);
}

// no updates needed for menu state
bool MenuState::update(sf::Time)
{
	return true;
}

// handle events, modifying nOptionIndex
bool MenuState::handleEvent(const sf::Event& event)
{
	// pass to guicontainer
	nGUIContainer.handleEvent(event);

	return false;
}




