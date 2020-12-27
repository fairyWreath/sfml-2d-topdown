#ifndef BUTTON_HPP
#define BUTTON_HPP

// button class with std::function for callbacks

#include "Component.hpp"
#include "ResourceIdentifiers.hpp"
#include "State.hpp"

// buttons have sprites for their backgrounds and a text label
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

#include <vector>
#include <string>
#include <memory>
#include <functional>			// std::function

// forward declare SoundPlayer
class SoundPlayer;

namespace GUI
{

class Button : public Component
{
public:
	typedef std::shared_ptr<Button> Ptr;			
	typedef std::function<void()> Callback;				// button functions are all void()

	enum ButtonState
	{
		Normal,
		Selected,
		Pressed			// activated
	};

public:
	// constructor, passing in fonts and textures
	Button(State::Context context);

	// set std::function callback
	void setCallback(Callback callback);

	// change button text
	void setText(const std::string& text);

	// button toggle
	void setToggle(bool flag);

	// overide default texture
	void setTexture(const sf::Texture& texture, ButtonState state);

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

	// textures, for memaddresses
	sf::Texture& nNormalTexture;			
	sf::Texture& nSelectedTexture;
	sf::Texture& nPressedTexture;

	// sfml members for display
	sf::Sprite nSprite;
	sf::Text nText;
	
	bool nIsToggle;

	SoundPlayer& nSounds;
};


}


#endif