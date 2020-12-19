#ifndef UTILITY_HPP
#define UTILITY_HPP

// utility functions

#include <sstream>			// string stream

#include <SFML/Window/Keyboard.hpp>

// forward declaration of sf classes
namespace sf
{
	class Sprite;
	class Text;
}

// convert to string
template <typename Type>
std::string toString(const Type& value);


// from Keyboard::Key to string
std::string toString(sf::Keyboard::Key key);


// center sfml objects in a window
void centerOrigin(sf::Sprite& sprite);
void centerOrigin(sf::Text& text);


// radian/degree conversion
float toDegree(float radian);
float toRadian(float degree);

#include "Utility.inl"
#endif