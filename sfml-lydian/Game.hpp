#ifndef GAME_HPP
#define GAME_HPP

// main game class

#include "World.hpp"
#include "StringHelpers.hpp"
#include "Player.hpp"


// again, inherit it from noncopyable
class Game : private sf::NonCopyable
{
public:
	Game();
	void run();


private:
	// internal update functions
	void processInput();			// process both realtime and events, using the player class
	void update(sf::Time elapsedTime);
	void render();				// render to window

	// helper functions 
	void updateStatistics(sf::Time elapsedTime);
	

private:
	static const sf::Time TimePerFrame;			// fps

	sf::RenderWindow nWindow;		
	World nWorld;

	// misc members, to store misc members
	sf::Font nFont;
	sf::Text nStatisticsText;
	sf::Time nStatisticsUpdateTime;
	std::size_t nStatisticsNumFrames;

	// pause state
	bool nIsPaused;

	// player
	Player nPlayer;		// player input handling is done inside Game::processInput

};

#endif