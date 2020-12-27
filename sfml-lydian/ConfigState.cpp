#include "ConfigState.hpp"
#include "Button.hpp"
#include "Utility.hpp"
#include "ResourceHolder.hpp"
#include "SoundPlayer.hpp"
#include "MusicPlayer.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

#include <iostream>

using namespace std::placeholders;

ConfigState::ConfigState(StateStack& stack, Context context) :
	State(stack, context),
	nSoundPlayer(context.soundPlayer), 
	nMusicPlayer(context.musicPlayer),
	nSoundEffectRects(),
	nMusicRects()
{
	nBackgroundSprite.setTexture(context.textures->get(Textures::SettingsScreen));

	// settings up the buttons
	auto increaseSoundButton = std::make_shared<GUI::Button>(context);
	increaseSoundButton->setPosition(386.5f, 550.f);
	increaseSoundButton->setText("Increase Sound");
	auto increaseSoundfn = [context]()
	{
		context.soundPlayer->modifyVolume(10.f);
	};
	increaseSoundButton->setCallback(increaseSoundfn);

	auto decreaseSoundButton = std::make_shared<GUI::Button>(context);
	decreaseSoundButton->setPosition(386.5f, 600.f);
	decreaseSoundButton->setText("Decrease Sound");
	auto decreaseSoundfn = [context]()
	{
		context.soundPlayer->modifyVolume(-10.f);
	};
	decreaseSoundButton->setCallback(decreaseSoundfn);

	auto increaseMusicButton = std::make_shared<GUI::Button>(context);
	increaseMusicButton->setPosition(386.5f, 650.f);
	increaseMusicButton->setText("Increase Music");
	auto increaseMusicfn = [context]()
	{
		context.musicPlayer->modifyVolume(10.f);
	};
	increaseMusicButton->setCallback(increaseMusicfn);

	auto decreaseMusicButton = std::make_shared<GUI::Button>(context);
	decreaseMusicButton->setPosition(386.5f, 700.f);
	decreaseMusicButton->setText("Decrease Music");
	auto decreaseMusicfn = [context]()
	{
		context.musicPlayer->modifyVolume(-10.f);
	};
	decreaseMusicButton->setCallback(decreaseMusicfn);

	auto backButton = std::make_shared<GUI::Button>(context);
	backButton->setPosition(0.f, 0.f);
	backButton->setText("Back");
	backButton->setCallback(std::bind(&ConfigState::requestStackPop, this));

	nGUIContainer.pack(increaseSoundButton);
	nGUIContainer.pack(decreaseSoundButton);
	nGUIContainer.pack(increaseMusicButton);
	nGUIContainer.pack(decreaseMusicButton);
	nGUIContainer.pack(backButton);

	updateUI();
}

void ConfigState::draw()
{
	// get window first
	sf::RenderWindow& window = *getContext().window;


	window.draw(nBackgroundSprite);
	window.draw(nGUIContainer);

	for (const auto& rect : nSoundEffectRects)
		window.draw(rect);

	for (const auto& rect : nMusicRects)
		window.draw(rect);
}

// no animations to update
bool ConfigState::update(sf::Time)
{
	updateUI();

	return true;
}

bool ConfigState::handleEvent(const sf::Event& event)
{

	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
	{
		requestStackPop();
	}

	nGUIContainer.handleEvent(event);

	return false;
}

void ConfigState::updateUI()
{
	int totalAmount = 10;

	int soundAmount = (int)nSoundPlayer->getVolume() / totalAmount - 1;
	int musicAmount = (int)nMusicPlayer->getVolume() / totalAmount - 1;

	nMusicRects.clear();
	nSoundEffectRects.clear();

	for (int i = 0; i < totalAmount; i++)
	{
		sf::RectangleShape rectangle;
		rectangle.setSize(sf::Vector2f(35, 44));
		sf::Color color = sf::Color(255, 132, 188);

		if (i > soundAmount)
			color = sf::Color(255, 187, 218);

		color.a = 210;
		rectangle.setFillColor(color);
		rectangle.setPosition(300.f + (40.f * i), 250.f);

		nSoundEffectRects.push_back(rectangle);
	}

	for (int i = 0; i < totalAmount; i++)
	{
		sf::RectangleShape rectangle;
		rectangle.setSize(sf::Vector2f(35, 44));
		sf::Color color = sf::Color(255, 132, 188);

		if (i > musicAmount)
			color = sf::Color(255, 187, 218);

		color.a = 210;
		rectangle.setFillColor(color);
		rectangle.setPosition(300.f + (40.f * i), 320.f);

		nMusicRects.push_back(rectangle);
	}
}




