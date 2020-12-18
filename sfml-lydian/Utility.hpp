#ifndef UTILITY_HPP
#define UTILITY_HPP

// utility functions

#include <sstream>			// string stream


// forward declaration of sf classes
namespace sf
{
	class Sprite;
	class Text;
}

// convert to string
template <typename Type>
std::string toString(const Type& value);

// center sfml objects in a window
void centerOrigin(sf::Sprite& sprite);
void centerOrigin(sf::Text& text);

#include "Utility.inl"
#endif