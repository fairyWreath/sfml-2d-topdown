#include "TitleState.hpp"
#include "ResourceHolder.hpp"
#include "Utility.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

TitleState::TitleState(StateStack& stack, Context context) :
	State(stack, context),
	nText(),
	nShowText(true),
	nTextEffectTime(sf::Time::Zero)		// set initial effect time to zero
{
	// use sprite settexture to get image, get textures from context
	nBackgroundSprite.setTexture(context.textures->get(Textures::TitleScreen));

	// again, set fonts from context
	nText.setFont(context.fonts->get(Fonts::Main));
	nText.setString("Start Game");
	centerOrigin(nText);
	nText.setPosition(context.window->getView().getSize() / 2.f);
}

// virtual functions of state to override

// drawing
void TitleState::draw()
{
	sf::RenderWindow& window = *getContext().window;			// get window memaddress from context
	// draw the background sprite
	window.draw(nBackgroundSprite);

	// show text
	if (nShowText)
		window.draw(nText);
}

// updating
bool TitleState::update(sf::Time dt)
{
	nTextEffectTime += dt;

	// blinking effect of the title text
	if (nTextEffectTime >= sf::seconds(0.5f))
	{
		nShowText = !nShowText;		// set showtext to opposite
		nTextEffectTime = sf::Time::Zero;			// reset effect time
	}

	return true;
}


bool TitleState::handleEvent(const sf::Event& event)
{
	// "press any button to start"
	if (event.type == sf::Event::KeyPressed)
	{
		// navigating between stacks
		requestStackPop();			// pop the current state, the title state from the stack

		// push main menu state to the stack
		requestStackPush(States::Menu);
	}

	return true;
}

