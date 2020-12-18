#ifndef LOADINGSTATE_HPP
#define LOADINGSTATE_HPP

// class for loading state

#include "State.hpp"
#include "ParallelTask.hpp"				// task threading

#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

class LoadingState : public State
{
public:
	// constructor to pass in values for State class constructor
	LoadingState(StateStack& stack, Context context);

	// virtual State functions
	virtual void draw();
	virtual bool update(sf::Time dt);
	virtual bool handleEvent(const sf::Event& event);

	// for progress bar, drawing how much the progress is
	void setCompletion(float percent);

private:
	// sfml members for display
	sf::Text nLoadingText;
	sf::RectangleShape nProgressBarBackground;
	sf::RectangleShape nProgressBar;

	// paralleltask class
	ParallelTask nLoadingTask;
};

#endif