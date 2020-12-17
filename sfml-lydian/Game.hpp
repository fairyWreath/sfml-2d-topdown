#ifndef GAME_HPP
#define GAME_HPP

// main game class

#include "World.hpp"
#include "StringHelpers.hpp"


// again, inherit it from noncopyable
class Game : private sf::NonCopyable
{
public:
	Game();
	void run();


private:
	// internal update functions
	void processEvents();
	void update(sf::Time elapsedTime);
	void render();				// render to window

	// helper functions 
	void updateStatistics(sf::Time elapsedTime);
	void handlePlayerInput(sf::Keyboard::Key key, bool isPressed);

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
	bool nIsPaused = false;

	// private members for gamelogic/movement
	bool nIsMovingUp;
	bool nIsMovingDown;
	bool nIsMovingLeft;
	bool nIsMovingRight;

};

#endif