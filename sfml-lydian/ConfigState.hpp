// settings for audio etc.
#ifndef GAMESETTINGSTATE_HPP
#define GAMESETTINGSTATE_HPP

#include "State.hpp"
#include "Container.hpp"

#include <vector>		// to store rectangles

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

class SoundPlayer;
class MusicPlayer;

class ConfigState : public State
{
public:
	enum Action
	{
		IncreaseSoundEffects,
		DecreaseSoundEffects,
		IncreaseMusic,
		DecreaseMusic,
		ActionCount
	};

public:
	// constructor with params for State class
	ConfigState(StateStack& stack, Context context);

	// virutal functions of State to override
	virtual void draw();
	virtual bool update(sf::Time dt);
	virtual bool handleEvent(const sf::Event& event);

private:
	void updateUI();

private:
	// volume display
	std::vector<sf::RectangleShape> nSoundEffectRects;
	std::vector<sf::RectangleShape> nMusicRects;

	sf::Sprite nBackgroundSprite;
	GUI::Container nGUIContainer;

	SoundPlayer* nSoundPlayer;
	MusicPlayer* nMusicPlayer;
};



#endif