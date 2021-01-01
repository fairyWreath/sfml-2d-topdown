#include "EditorState.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

EditorState::EditorState(StateStack& stack, Context context) :
	State(stack, context),
	nBackgroundSprite()
{

}


void EditorState::draw()
{
	// get window memaddress from context
	sf::RenderWindow& window = *getContext().window;

	window.setView(window.getDefaultView());		// set viewport to default
	window.draw(nBackgroundSprite);				// draw the sprite

	// draw container
	window.draw(nGUIContainer);
}

bool EditorState::update(sf::Time)
{
	return true;
}

bool EditorState::handleEvent(const sf::Event& event)
{
	nGUIContainer.handleEvent(event);

	return false;
}
