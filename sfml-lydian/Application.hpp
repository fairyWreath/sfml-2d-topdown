#ifndef APPLICATION_HPP
#define APPLICATION_HPP

/*	application class, controls:
1. Inputs
2. logic upodates
3. Rendering
*/

#include "ResourceHolder.hpp"
#include "ResourceIdentifiers.hpp"
#include "Player.hpp"
#include "StateStack.hpp"
#include "MusicPlayer.hpp"

#include <SFML/System/Time.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>


// mimics previous game class
class Application
{
public:
	Application();
	void run();					// main run() function

private:
	// internal update functions
	void processInput();			// process both realtime and events, using the player class
	void update(sf::Time elapsedTime);
	void render();				// render to window

	// helper functions 
	void updateStatistics(sf::Time elapsedTime);

	// initializing/registering states
	void registerStates();


private:
	static const sf::Time TimePerFrame;			// fps

	sf::RenderWindow nWindow;
	
	// class members from previous World class
	TextureHolder nTextures;
	FontHolder nFonts;
	Player nPlayer;		// player input handling is done inside Game::processInput
	MusicPlayer nMusicPlayer;
	SoundPlayer nSoundPlayer;

	// the state stack
	StateStack nStateStack;

	// misc members, to store misc members
	sf::Text nStatisticsText;
	sf::Time nStatisticsUpdateTime;
	std::size_t nStatisticsNumFrames;

};



#endif