#include "GameEndState.hpp"
#include "Utility.hpp"
#include "Player.hpp"
#include "ResourceHolder.hpp"
#include "Label.hpp"
#include "Button.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>

GameEndState::GameEndState(StateStack& stack, Context context) :
	State(stack, context),
	nElapsedTime(sf::Time::Zero),
	nBackgroundSprite(context.textures->get(Textures::GameEndScreen))
{
	auto endLabel = std::make_shared<GUI::Label>("Game Over", *context.fonts);
	endLabel->setPosition(400.f, 100.f);
	endLabel->setSize(120);

	endLabel->setFont(context.fonts->get(Fonts::Title));		// set custom font

	auto infoLabel = std::make_shared<GUI::Label>("You are Dead", *context.fonts);
	if (context.player->getMissionStatus() == Player::MissionSuccess)
		infoLabel->setText("Enjoy Life");
	infoLabel->setColor(sf::Color(255, 132, 188));
	infoLabel->setPosition(550.f, 270.f);


	nGUIContainer.pack(endLabel);
	nGUIContainer.pack(infoLabel);
}

void GameEndState::draw()
{
	sf::RenderWindow& window = *getContext().window;
	window.setView(window.getDefaultView());

	window.draw(nBackgroundSprite);
	window.draw(nGUIContainer);
}

bool GameEndState::update(sf::Time dt)
{
	nElapsedTime += dt;

	//if (nElapsedTime.asSeconds() > 4)
	//{
	//	
	//}

	return false;
}

bool GameEndState::handleEvent(const sf::Event& event)
{
	if (nElapsedTime.asSeconds() > 3)
	{
		if (sf::Event::KeyPressed == event.type)
		{
			// go back to title
			requestStateClear();
			requestStackPush(States::Title);
		}
	}

	nGUIContainer.handleEvent(event);
	return false;
}