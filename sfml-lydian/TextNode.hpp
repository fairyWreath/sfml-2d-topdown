#ifndef TEXTNODE_HPP
#define TEXTNODE_HPP

// class for displaying ingame text

#include "ResourceHolder.hpp"
#include "ResourceIdentifiers.hpp"
#include "SceneNode.hpp"

#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Font.hpp>

class TextNode : public SceneNode
{
public:
	// pass in fonts and text to display
	explicit TextNode(const FontHolder& fonts, const std::string& text);

	// change text
	void setString(const std::string& text);

private:
	// override SceneNode drawcurrent
	virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	// sfml class to hold the text
	sf::Text nText;

};

#endif