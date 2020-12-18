#include "LoadingState.hpp"
#include "Utility.hpp"
#include "ResourceHolder.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>

LoadingState::LoadingState(StateStack& stack, Context context) :
	State(stack, context)
{
	// get resources from context
	sf::RenderWindow& window = *getContext().window;
	sf::Font& font = context.fonts->get(Fonts::Main);
	sf::Vector2f viewSize = window.getView().getSize();

	// set texts and progressbar
	nLoadingText.setFont(font);
	nLoadingText.setString("Loading Resources");
	centerOrigin(nLoadingText);
	nLoadingText.setPosition(viewSize.x / 2.f, viewSize.y / 2.f);

	// progress bar background
	nProgressBarBackground.setFillColor(sf::Color::White);			// set background to white
	nProgressBarBackground.setSize(sf::Vector2f(viewSize.x - 20, 10));			// set length and width of rectangle
	nProgressBarBackground.setPosition(10, nLoadingText.getPosition().y + 40);			// set below text

	// progress bar itself, on top of the progress bar background
	nProgressBar.setFillColor(sf::Color(100, 100, 100));
	nProgressBar.setSize(sf::Vector2f(200, 10));
	nProgressBar.setPosition(10, nLoadingText.getPosition().y + 40);

	// set initial completion rate to zero
	setCompletion(0.f);

	//run paralel task trhead
	nLoadingTask.execute();
}

// virtual functions to draw, update handle events
void LoadingState::draw()
{
	sf::RenderWindow& window = *getContext().window;			// get window pointer from context
	window.setView(window.getDefaultView());

	// draw the sfml members
	window.draw(nLoadingText);
	window.draw(nProgressBarBackground);
	window.draw(nProgressBar);
}


bool LoadingState::update(sf::Time dt)
{
	// update based on the parallel task class
	if (nLoadingTask.isFinished())			// if loading is finished
	{
		// pop loading state and go to game
		requestStackPop();
		requestStackPush(States::Game);
	}
	else
	{
		// update progress bar, getting info from the paralleltask class
		setCompletion(nLoadingTask.getCompletion());
	}
	return true;
}

// no events for loading state
bool LoadingState::handleEvent(const sf::Event& event)
{
	return true;
}


// drawing the progress bar, based on % completion
void LoadingState::setCompletion(float percent)
{
	if (percent > 1.f)		// clamp to max, 100%
		percent = 1.f;

	// draw based on percentage
	nProgressBar.setSize(sf::Vector2f(nProgressBarBackground.getSize().x * percent, nProgressBarBackground.getSize().y));
}




