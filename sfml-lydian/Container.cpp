#include "Container.hpp"

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include <iostream>
#include <cassert>

namespace GUI
{

Container::Container() :
	nChildren(),
	nSelectedChild(-1)		// set currently selected index to none
{
}

// placing component to the vector
void Container::pack(Component::Ptr component)
{
	nChildren.push_back(component);			// insert to component

	// if a child is currently selected and if new added component is selected, add it
//	if (!hasSelection() && component->isSelectable())
//		select(nChildren.size() - 1);
}

// container classes cannot be selected
bool Container::isSelectable() const
{
	return false;
}

// handling events of components inside the container
void Container::handleEvent(const sf::Event& event)
{
	for (auto& child : nChildren)
	{
		child->handleEvent(event);
	}

	// if a child is already selected and it is active, let it handle the event
	//if (hasSelection() && nChildren[nSelectedChild]->isActive())
	//{
	//	nChildren[nSelectedChild]->handleEvent(event);
	//}
	//// if none is currently selected, navitage components with keyboard
	//else if (event.type == sf::Event::KeyReleased)	
	//{
	//	// w or uparrow
	//	if (event.key.code == sf::Keyboard::W	
	//		|| event.key.code == sf::Keyboard::Up)
	//	{
	//		selectPrevious();			// select previous component
	//	}

	//	// s or down
	//	else if (event.key.code == sf::Keyboard::S
	//		|| event.key.code == sf::Keyboard::Down)
	//	{
	//		selectNext();			// select previous component
	//	}

	//	// additional left or right mimicking up or down
	//	else if (event.key.code == sf::Keyboard::D
	//		|| event.key.code == sf::Keyboard::Right)
	//	{
	//		selectNext();			// select previous component
	//	}
	//	else if (event.key.code == sf::Keyboard::A
	//		|| event.key.code == sf::Keyboard::Left)
	//	{
	//		selectPrevious();			// select previous component
	//	}

	//	// space or enter to activate the selected component
	//	else if (event.key.code == sf::Keyboard::Return
	//		|| event.key.code == sf::Keyboard::Space)
	//	{
	//		if (hasSelection())
	//			nChildren[nSelectedChild]->activate();
	//	}

	//}
}


// navigate container components
void Container::select(std::size_t index)
{
	if (nChildren[index]->isSelectable())		// check if selectable
	{
		if (hasSelection())		// first deselect if a component is already selected
			nChildren[nSelectedChild]->deselect();
		nChildren[index]->select();			// select index
		nSelectedChild = index;
	}
}

// select next component
void Container::selectNext()
{
	if (!hasSelection())		// return if no selection is currently being made
		return;

	// search for next component that is selectable
	int next = nSelectedChild;
	do
		next = (next + 1) % nChildren.size();			// basically next++ but returns to 0 if reached end
	while (!nChildren[next]->isSelectable());		// stop if child is selectable

	// select that component
	select(next);
}

void Container::selectPrevious()
{
	if (!hasSelection())		// return if no selection is currently being made
		return;

	// search for next component that is selectable
	int prev = nSelectedChild;
	do
		prev = (prev + nChildren.size() - 1) % nChildren.size();			// basically next- but returns to 0 if reached end
	while (!nChildren[prev]->isSelectable());		// stop if child is selectable

	// select that component
	select(prev);
}


// if a selection is being made
bool Container::hasSelection() const
{
	return nSelectedChild >= 0;			// if index is not -1, a child is selected
}


// drawing the container components
void Container::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	// get transform from sf::Transform and append it with overlaoded *
	states.transform *= getTransform();

	if (nHasBackground)
		target.draw(*nBackground, states);

	// draw all components
	for (const auto& child : nChildren)
		target.draw(*child, states);
}



void Container::setBackground(bool hasBackground)
{
	nHasBackground = hasBackground;

	if (nHasBackground && nBackground == nullptr)
	{
		nBackground = std::make_unique<sf::RectangleShape>();
	}
}

void Container::setBackgroundColor(sf::Color color)
{
	assert(nHasBackground);

	nBackground->setFillColor(color);
}

void Container::setBackgroundOutlineColor(sf::Color color)
{
	assert(nHasBackground);

	nBackground->setOutlineThickness(2.f);
	nBackground->setOutlineColor(color);
}

void Container::setBackgroundSize(float x, float y)
{
	assert(nHasBackground);

	nBackground->setSize(sf::Vector2f(x, y));
}

void Container::setBackgroundPosition(float x, float y)
{
	assert(nHasBackground);

	// set transformable position
	//setPosition(x, y);

	nBackground->setPosition(x, y);
}

}