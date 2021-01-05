#include "Component.hpp"

namespace GUI
{

// base class constructor
Component::Component() :
	nIsSelected(false),
	nIsActive(false)
{
}


// virtual destructor
Component::~Component()
{
}

// return selected state
bool Component::isSelected() const
{
	return nIsSelected;
}

// return active state
bool Component::isActive() const
{
	return nIsActive;
}

/* select/activate changes
- works by changing the bool nIsSelected and nIsActive states
*/


void Component::select()
{
	nIsSelected = true;
}

void Component::deselect()
{
	nIsSelected = false;
}

void Component::activate()
{
	nIsActive = true;
}


void Component::deactivate()
{
	nIsActive = false;
}




}

