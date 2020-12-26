#include "TitleState.hpp"
#include "Label.hpp"
#include "Utility.hpp"
#include "ResourceHolder.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

TitleState::TitleState(StateStack& stack, Context context) :
	State(stack, context),
	nText(),
	nGUIContainer(),
	nShowText(true),
	nTextEffectTime(sf::Time::Zero)		// set initial effect time to zero
{
	// use sprite settexture to get image, get textures from context
	nBackgroundSprite.setTexture(context.textures->get(Textures::TitleScreen));

	// create title label
	auto titleLabel = std::make_shared<GUI::Label>("Easy Carefree Existence", *context.fonts);
	titleLabel->setPosition(250, 100);
	titleLabel->setSize(120);
	titleLabel->setColor(sf::Color(164, 231, 223));
	titleLabel->setFont(context.fonts->get(Fonts::Title));		// set custom font

	nGUIContainer.pack(titleLabel);

	//  again, set fonts from context
	nText.setFont(context.fonts->get(Fonts::Main));
	nText.setString("Press Any Key to Start");
	centerOrigin(nText);
	nText.setPosition(context.window->getView().getSize().x / 2.f, 500.f);

	context.musicPlayer->play(Music::MainTheme);
}

// virtual functions of state to override

// drawing
void TitleState::draw()
{
	sf::RenderWindow& window = *getContext().window;			// get window memaddress from context
	// draw the background sprite
	window.draw(nBackgroundSprite);

	window.draw(nGUIContainer);

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

	nGUIContainer.handleEvent(event);

	return true;
}

