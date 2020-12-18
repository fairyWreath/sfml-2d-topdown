#ifndef LABEL_HPP
#define LABEL_HPP

// label class, text widgets

#include "Component.hpp"
#include "ResourceIdentifiers.hpp"
#include "ResourceHolder.hpp"

#include <SFML/Graphics/Text.hpp>

namespace GUI
{

class Label : public Component
{
public:
	typedef std::shared_ptr<Label> Ptr;
	
public:
	// constructor, pass in string label and the font holder
	Label(const std::string& text, const FontHolder& fonts);


	// override virtual functions
	virtual bool isSelectable() const;
	virtual void handleEvent(const sf::Event& event);

	// change text
	void setText(const std::string& text);

private:
	// draw to dinwo
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	// sfml member to hold the text
	sf::Text nText;

};

}






#endif