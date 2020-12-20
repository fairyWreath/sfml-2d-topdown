#include "TextNode.hpp"
#include "Utility.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

TextNode::TextNode(const FontHolder& fonts, const std::string& text)
{
	nText.setFont(fonts.get(Fonts::Main));		// load from resourceholder
	nText.setCharacterSize(22);
	nText.setString(text);
	nText.setFillColor(sf::Color::Magenta);
}

void TextNode::setString(const std::string& text)
{
	nText.setString(text);
	centerOrigin(nText);
}


// draw the text
void TextNode::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(nText, states);			// pass in the sf::Text and states
}
