#ifndef CONTAINER_HPP
#define CONTAINER_HPP

// base class for ui container

#include "Component.hpp"

#include <SFML/Graphics/RectangleShape.hpp>

#include <vector>
#include <memory>		//shared_ptr

namespace GUI
{

class Container : public Component
{
public:
	typedef std::shared_ptr<Container> Ptr;

public:
	Container();

	// pack component into the container
	void pack(Component::Ptr component);			// accept shared_ptr of a component
	
	// virtual functions to override
	virtual bool isSelectable() const;
	virtual void handleEvent(const sf::Event& event);



	void setBackground(bool hasBackground);

	void setBackgroundColor(sf::Color color);
	void setBackgroundOutlineColor(sf::Color color);
	void setBackgroundSize(float x, float y);
	void setBackgroundPosition(float x, float y);

private:
	// function to draw components
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;		// again, with these parametsr, we can call window.draw(container)

	// selection of the components inside
	bool hasSelection() const;
	void select(std::size_t index);			// select a single component based on index
	void selectNext();
	void selectPrevious();



private:
	std::vector<Component::Ptr> nChildren;			// vector to store component shared_ptrs
	int nSelectedChild;						// currently selected component


	bool nHasBackground;
	std::unique_ptr<sf::RectangleShape> nBackground;
};





}

#endif