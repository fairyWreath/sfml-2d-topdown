#ifndef COMMAND_HPP
#define COMMAND_HPP


#include <SFML/System/Time.hpp>

#include <functional>		// std::function
#include <cassert>

#include "Category.hpp"			// category enums

// base struct class for std:function callbacks

// forward declare SceneNode class
class SceneNode;

struct Command
{
	Command();			// constructor for command

	/*		std::function, class template for treating functions as callback objects
	- return type void
	- parameters SceneNode& and sf::Time
	- can used std::bind to bound function arguments to given values
	*/
	std::function<void(SceneNode&, sf::Time)> action;	

	unsigned int category;				// recipient category of the command, based on Category enum
};


// function to downcasts to derived SceneNode classes
template <typename GameObject, typename Function>
std::function<void(SceneNode&, sf::Time)> derivedAction(Function fn)		// pass in function type
{
	// lambda expression
	return [=](SceneNode& node, sf::Time dt)
	{
		// check downcast is save
		assert(dynamic_cast<GameObject*>(&node) != nullptr);			// to be converted to GameObject type

		// downcast node and invoke function on it
		fn(static_cast<GameObject&>(node), dt);
	};
}



#endif