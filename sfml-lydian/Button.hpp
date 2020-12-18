#ifndef BUTTON_HPP
#define BUTTON_HPP

// button class with std::function for callbacks

#include "Component.hpp"
#include "ResourceIdentifiers.hpp"
#include "ResourceHolder.hpp"

// buttons have sprites for their backgrounds and a text label
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

#include <vector>
#include <string>
#include <memory>
#include <functional>			// std::function


namespace GUI
{

class Button : public Component
{
public:
	typedef std::shared_ptr<Button> Ptr;			
	typedef std::function<void()> Callback;				// button functions are all void()

public:
	// constructor, passing in fonts and textures
	Button(const FontHolder& fonts, const TextureHolder& textures);

	// set std::function callback
	void setCallback(Callback callback);

	// change button text
	void setText(const std::string& text);

	// button toggle
	void setToggle(bool flag);

	// virtual override functions from Component class
	virtual bool isSelectable() const;	

	virtual void select();
	virtual void deselect();
	virtual void activate();
	virtual void deactivate();

	virtual void handleEvent(const sf::Event& event);

private:
	// window draw function
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	Callback nCallback;			// std::function

	// texxtures, for memaddresses
	const sf::Texture& nNormalTexture;			
	const sf::Texture& nSelectedTexture;
	const sf::Texture& nPressedTexture;

	// sfml members for display
	sf::Sprite nSprite;
	sf::Text nText;
	
	bool nIsToggle;
};


}


#endif