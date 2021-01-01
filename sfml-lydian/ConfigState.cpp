#include "ConfigState.hpp"
#include "Button.hpp"
#include "Utility.hpp"
#include "ResourceHolder.hpp"
#include "SoundPlayer.hpp"
#include "MusicPlayer.hpp"
#include "Label.hpp"

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


	sf::Color normalColor(sf::Color(50, 198, 182));
	sf::Color selectedColor(sf::Color(164, 231, 223));
	normalColor.a = 205;
	selectedColor.a = 205;

	/* increase sounds */
	auto increaseSoundButton = std::make_shared<GUI::Button>(context);
	increaseSoundButton->setType(GUI::Button::Shape);
					// set position first

	auto increaseSoundTriangle = std::make_unique<sf::CircleShape>(22, 3);
	increaseSoundTriangle->setRotation(90.f);
	increaseSoundButton->setShape(std::move(increaseSoundTriangle));
	increaseSoundButton->setPosition(820.f, 200.f);
	increaseSoundButton->setShapeColor(normalColor, GUI::Button::Normal);
	increaseSoundButton->setShapeColor(selectedColor, GUI::Button::Selected);
	increaseSoundButton->setShapeColor(selectedColor, GUI::Button::Pressed);
	auto increaseSoundfn = [context]()
	{
		context.soundPlayer->modifyVolume(10.f);
	};
	increaseSoundButton->setCallback(increaseSoundfn);

	/* decrease sounds */
	auto decreaseSoundButton = std::make_shared<GUI::Button>(context);
	decreaseSoundButton->setType(GUI::Button::Shape);

	auto decreaseSoundTriangle = std::make_unique<sf::CircleShape>(22, 3);
	decreaseSoundTriangle->setRotation(-90.f);
	decreaseSoundButton->setShape(std::move(decreaseSoundTriangle));
	decreaseSoundButton->setPosition(335.f, 244.f);
	decreaseSoundButton->setShapeColor(normalColor, GUI::Button::Normal);
	decreaseSoundButton->setShapeColor(selectedColor, GUI::Button::Selected);
	decreaseSoundButton->setShapeColor(selectedColor, GUI::Button::Pressed);
	auto decreaseSoundfn = [context]()
	{
		context.soundPlayer->modifyVolume(-10.f);
	};
	decreaseSoundButton->setCallback(decreaseSoundfn);


	/* increase music */
	auto increaseMusicButton = std::make_shared<GUI::Button>(context);
	increaseMusicButton->setType(GUI::Button::Shape);

	auto increaseMusicTriangle = std::make_unique<sf::CircleShape>(22, 3);
	increaseMusicTriangle->setRotation(90.f);
	increaseMusicButton->setShape(std::move(increaseMusicTriangle));
	increaseMusicButton->setPosition(820.f, 270.f);
	increaseMusicButton->setShapeColor(normalColor, GUI::Button::Normal);
	increaseMusicButton->setShapeColor(selectedColor, GUI::Button::Selected);
	increaseMusicButton->setShapeColor(selectedColor, GUI::Button::Pressed);
	auto increaseMusicfn = [context]()
	{
		context.musicPlayer->modifyVolume(10.f);
	};
	increaseMusicButton->setCallback(increaseMusicfn);


	/* decrease music */
	auto decreaseMusicButton = std::make_shared<GUI::Button>(context);
	decreaseMusicButton->setType(GUI::Button::Shape);
	
	auto decreaseMusicTriangle = std::make_unique<sf::CircleShape>(22, 3);
	decreaseMusicTriangle->setRotation(-90.f);
	decreaseMusicButton->setShape(std::move(decreaseMusicTriangle));
	decreaseMusicButton->setPosition(335.f, 314.f);
	decreaseMusicButton->setShapeColor(normalColor, GUI::Button::Normal);
	decreaseMusicButton->setShapeColor(selectedColor, GUI::Button::Selected);
	decreaseMusicButton->setShapeColor(selectedColor, GUI::Button::Pressed);
	auto decreaseMusicfn = [context]()
	{
		context.musicPlayer->modifyVolume(-10.f);
	};
	decreaseMusicButton->setCallback(decreaseMusicfn);

	auto backButton = std::make_shared<GUI::Button>(context);
	backButton->setPosition(50.f, 600.f);
	backButton->setText("Back");
	backButton->setCallback(std::bind(&ConfigState::requestStackPop, this));

	auto configLabel = std::make_shared<GUI::Label>("Config", *context.fonts);
	configLabel->setPosition(80.f, 20.f);
	configLabel->setColor(sf::Color(164, 231, 223));
	configLabel->setFont(context.fonts->get(Fonts::Title));		// delanova
	configLabel->setSize(120);

	auto soundLabel = std::make_shared<GUI::Label>("Sound Effects", *context.fonts);
	soundLabel->setPosition(50.f, 200.f);
	soundLabel->setColor(sf::Color(255, 132, 188));
	soundLabel->setSize(40);
	

	auto musicLabel = std::make_shared<GUI::Label>("Music", *context.fonts);
	musicLabel->setPosition(50.f, 270.f);
	musicLabel->setColor(sf::Color(255, 132, 188));
	musicLabel->setSize(40);

	nGUIContainer.pack(increaseSoundButton);
	nGUIContainer.pack(decreaseSoundButton);
	nGUIContainer.pack(increaseMusicButton);
	nGUIContainer.pack(decreaseMusicButton);
	nGUIContainer.pack(backButton);
	nGUIContainer.pack(configLabel);
	nGUIContainer.pack(soundLabel);
	nGUIContainer.pack(musicLabel);


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


// update rectangles
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
		rectangle.setPosition(380.f + (40.f * i), 200.f);

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
		rectangle.setPosition(380.f + (40.f * i), 270.f);

		nMusicRects.push_back(rectangle);
	}


}




