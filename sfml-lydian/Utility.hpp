#ifndef UTILITY_HPP
#define UTILITY_HPP

// utility functions

#include <sstream>			// string stream

#include <SFML/Window/Keyboard.hpp>
#include <SFML/System/Vector2.hpp>

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

// vector operations
float length(sf::Vector2f vector);			// get modulus
sf::Vector2f unitVector(sf::Vector2f vector);		// get unit vector, direction / length


// radian/degree conversion
float toDegree(float radian);
float toRadian(float degree);


// generate random int
int randomInt(int exclusiveMax);

// get distance between


#include "Utility.inl"
#endif