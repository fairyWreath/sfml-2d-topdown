#ifndef COMPONENT_HPP
#define COMPONENT_HPP

// base class for gui

#include <SFML/System/NonCopyable.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>

#include <memory>


// forward declare sf::Event
namespace sf
{
	class Event;
}

// base class, encapsulated in gui namespace
namespace GUI
{

class Component : public sf::Drawable, public sf::Transformable, private sf::NonCopyable
{
public:
	typedef std::shared_ptr<Component> Ptr;			// use shared_ptr to contain components
	/* use shared_ptr for flexibility
	- component may be used in different states
	*/

public:
	Component();		// base class constructor

	// virtual function for child classes
	virtual ~Component();			// destructor

	virtual bool isSelectable() const = 0;		// for containers/buttons, assume false
	bool isSelected() const;

	virtual void select();
	virtual void deselect();
	virtual bool isActive() const;
	virtual void activate();
	virtual void deactivate();

	// events, handled by child classes
	virtual void handleEvent(const sf::Event& event) = 0;




private:
	bool nIsSelected;
	bool nIsActive;
};



}


#endif