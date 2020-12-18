#include "PauseState.hpp"
#include "Utility.hpp"
#include "ResourceHolder.hpp"

#include <SFML/Graphics/RectangleShape.hpp>		
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>

#include <iostream>			// debugging

PauseState::PauseState(StateStack& stack, Context context) :
	State(stack, context),
	nBackgroundSprite(),
	nPausedText(),
	nInstructionText()
{
	// load resources from context
	sf::Font& font = context.fonts->get(Fonts::Main);
	sf::Vector2f viewSize = context.window->getView().getSize();
	// set backgroundpsirte
	nBackgroundSprite.setTexture(context.textures->get(Textures::PauseScreen));

	// set sf::texts
	nPausedText.setFont(font);
	nPausedText.setString("Game is Paused");
	nPausedText.setCharacterSize(70);
	nPausedText.setFillColor(sf::Color::Cyan);
	centerOrigin(nPausedText);
	nPausedText.setPosition(0.5f * viewSize.x, 0.4f * viewSize.y);

	nInstructionText.setFont(font);
	nInstructionText.setString("Press Backspace to return to main menu");
	nInstructionText.setFillColor(sf::Color::Cyan);
	centerOrigin(nInstructionText);
	nInstructionText.setPosition(0.5f * viewSize.x, 0.6f * viewSize.y);
}


// virtual functions to handle events, draw, etc.
void PauseState::draw()
{
	// get render window from context
	sf::RenderWindow& window = *getContext().window;
	window.setView(window.getDefaultView());			// set view to window default

	// draw the background, a rectangle
	//sf::RectangleShape backgroundShape;
	//backgroundShape.setFillColor(sf::Color::Cyan);
	//backgroundShape.setSize(window.getView().getSize());
	//window.draw(backgroundShape);
	
	// draw sprite background
	window.draw(nBackgroundSprite);

	window.draw(nPausedText);
	window.draw(nInstructionText);
}

// no updates for pause state
bool PauseState::update(sf::Time)
{
	return false;
}

bool PauseState::handleEvent(const sf::Event& event)
{
	if (event.type != sf::Event::KeyPressed)
		return false;

	// escape pause state if esc key is pressed
	if (event.key.code == sf::Keyboard::Escape)
	{
		// pop pause state from the stacl
		requestStackPop();
		// std::cout << "Esc is pressed, return to game\n";
	}

	// backspace to go to main menu
	if (event.key.code == sf::Keyboard::BackSpace)
	{
		// clear and go to menu
		requestStateClear();
		requestStackPush(States::Menu);
		// std::cout << "Bkscp is pressed, return to menu\n";
	}

	return false;		// return false to pause the screen
}