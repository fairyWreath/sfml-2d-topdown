#include "Label.hpp"

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>


namespace GUI
{

Label::Label(const std::string& text, const FontHolder& fonts) :
	nText(text, fonts.get(Fonts::Label), 30)			// set string, font and size to sf::Text, get font from fontholder
{
	// set initial text color
	nText.setFillColor(sf::Color::Magenta);
}


// label is not selectable
bool Label::isSelectable() const
{
	return false;
}

// no event to handle
void Label::handleEvent(const sf::Event&)
{
}

// change text
void Label::setText(const std::string& text)
{
	nText.setString(text);			// change sf::Text string
}

// allow window to draw
void Label::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	// get and add transforms
	states.transform *= getTransform();
	target.draw(nText, states);
}

// functions to modify label, overloading, for custom lables
void Label::setFont(sf::Font& font)
{
	nText.setFont(font);
}

// set custom size
void Label::setSize(const float& size)
{
	nText.setCharacterSize(size);
}

// set custom label
void Label::setColor(const sf::Color& color)
{
	nText.setFillColor(color);
}


}