#include "MenuState.hpp"
#include "ResourceHolder.hpp"
#include "Utility.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>

// MenuState constructor
MenuState::MenuState(StateStack& stack, Context context) :
	State(stack, context),
	nOptions(),
	nOptionIndex(0)		// set to enum Play the first time
{
	// get sfml resources from context
	sf::Texture& texture = context.textures->get(Textures::TitleScreen);
	sf::Font& font = context.fonts->get(Fonts::Main);

	// set background sprite
	nBackgroundSprite.setTexture(texture);

	// creating the menu
	sf::Text playOption;		// sf::text type to display
	playOption.setFont(font);
	playOption.setString("Play");
	centerOrigin(playOption);
	playOption.setPosition(context.window->getView().getSize() / 2.f);		// set to middle
	nOptions.push_back(playOption);			// push back to text vector

	sf::Text exitOption;		// sf::text type to display
	playOption.setFont(font);
	playOption.setString("Exit");
	centerOrigin(exitOption);
	playOption.setPosition(playOption.getPosition() + sf::Vector2f(0.f, 30.f));		// place beloww the play option text		
	nOptions.push_back(playOption);			// push back to text vector

	updateOptionText();		// render text to window
}

// function to control the menu, set display based on currently selected text
void MenuState::updateOptionText()
{
	// if vector for text is empty do nothing
	if (nOptions.empty())
		return;

	// white all texts
	for (auto& text : nOptions)
	{
		text.setFillColor(sf::Color::White);
	}

	// magenta currently selected text
	nOptions[nOptionIndex].setFillColor(sf::Color::Magenta);
}


// override virtual functions for update, draw and handling events
void MenuState::draw()
{
	// get window memaddress from context
	sf::RenderWindow& window = *getContext().window;

	window.setView(window.getDefaultView());		// set viewport to default
	window.draw(nBackgroundSprite);				// draw the sprite

	// draw the texts from text vector
	for (const auto& text : nOptions)
		window.draw(text);
}

// no updates needed for menu state
bool MenuState::update(sf::Time)
{
	return true;
}

// handle events, modifying nOptionIndex
bool MenuState::handleEvent(const sf::Event& event)
{
	// menu logic
	if (event.type != sf::Event::KeyPressed)		// event has to be keypressed type
		return false;

	// if enter is pressed
	if (event.key.code == sf::Keyboard::Return)
	{
		if (nOptionIndex == Play)
		{
			requestStackPop();			// pop menu state from stack
			requestStackPush(States::Game);			// push game state
		}

		if (nOptionIndex == Exit)
		{
			// empty stack, game will close
			requestStackPop();
		}
	}

	// select top option
	else if (event.key.code == sf::Keyboard::Up)
	{
		if (nOptionIndex > 0)
			nOptionIndex--;
		else
			nOptionIndex = nOptions.size() - 1;		// 'go to back of the text container'

		updateOptionText();
	}

	// select bottom option
	else if (event.key.code == sf::Keyboard::Down)
	{
		if (nOptionIndex < nOptions.size() - 1)
			nOptionIndex++;
		else
			nOptionIndex = 0;		// 'go to back of the text container'

		updateOptionText();
	}



	return true;
}




