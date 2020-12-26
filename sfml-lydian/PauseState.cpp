#include "PauseState.hpp"
#include "Button.hpp"
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
	nGUIContainer()
{
	// load resources from context
	sf::Font& font = context.fonts->get(Fonts::Main);
	sf::Vector2f viewSize = context.window->getView().getSize();
	nBackgroundSprite.setTexture(context.textures->get(Textures::PauseScreen));

	// set sf::texts
	nPausedText.setFont(font);
	nPausedText.setString("Game is Paused");
	nPausedText.setCharacterSize(70);
	nPausedText.setFillColor(sf::Color::Cyan);
	centerOrigin(nPausedText);
	nPausedText.setPosition(0.5f * viewSize.x, 0.4f * viewSize.y);


	// windowsize from texture, convert from Vector2u to Vector2f
	sf::Vector2f windowSize(context.window->getSize());

	// set up return button
	auto returnButton = std::make_shared<GUI::Button>(*context.fonts, *context.textures);
	returnButton->setPosition(0.5f * windowSize.x - 253.5f, 0.4f * windowSize.y + 125.f);
	returnButton->setText("Return");
	returnButton->setCallback([this]()
		{
			// pop pause state
			requestStackPop();
		});

	auto settingsButton = std::make_shared<GUI::Button>(*context.fonts, *context.textures);
	settingsButton->setPosition(0.5f * windowSize.x - 253.5, 0.4f * windowSize.y + 205.f);
	settingsButton->setText("Settings");
	settingsButton->setCallback([this]()			// request state push is this class's(state) function, use this capture clause
		{
			// push settings state
			requestStackPush(States::Settings);
		});

	auto mainMenuButton = std::make_shared<GUI::Button>(*context.fonts, *context.textures);
	mainMenuButton->setPosition(0.5f * windowSize.x - 253.5, 0.4f * windowSize.y + 285.f);
	mainMenuButton->setText("Main Menu");
	mainMenuButton->setCallback([this]()
		{
			// clear all states and go to menu
			requestStateClear();
			requestStackPush(States::Menu);
		});

	nGUIContainer.pack(returnButton);
	nGUIContainer.pack(settingsButton);
	nGUIContainer.pack(mainMenuButton);


	nPreviousMusic = context.musicPlayer->getCurrentMusic();
	nPreviousMusicOffset = context.musicPlayer->getCurrentMusicOffset();
	context.musicPlayer->play(Music::SecondaryTheme);		// play pause theme
}

PauseState::~PauseState()
{
	// getContext().musicPlayer->setPaused(false);
	
	// play when returning to game
	getContext().musicPlayer->play(Music::MissionTheme);		// play pause theme
	getContext().musicPlayer->setCurrentMusicOffset(nPreviousMusicOffset);
}


// virtual functions to handle events, draw, etc.
void PauseState::draw()
{
	// get render window from context
	sf::RenderWindow& window = *getContext().window;
	window.setView(window.getDefaultView());			// set view to window default
	
	// draw sprite background
	window.draw(nBackgroundSprite);

	window.draw(nPausedText);

	window.draw(nGUIContainer);
}

// no updates for pause state
bool PauseState::update(sf::Time)
{
	return false;
}

bool PauseState::handleEvent(const sf::Event& event)
{
	/*if (event.type != sf::Event::KeyPressed)
		return false;*/

	// escape pause state if esc key is pressed
	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
	{
		// pop pause state from the stacl
		requestStackPop();
		// std::cout << "Esc is pressed, return to game\n";
	}

	nGUIContainer.handleEvent(event);

	return false;		// return false to pause the screen
}