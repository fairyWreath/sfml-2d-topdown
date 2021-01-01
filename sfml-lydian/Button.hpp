#ifndef BUTTON_HPP
#define BUTTON_HPP

// button class with std::function for callbacks

#include "Component.hpp"
#include "ResourceIdentifiers.hpp"
#include "State.hpp"

// buttons have sprites for their backgrounds and a text label
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/CircleShape.hpp>

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

	enum ButtonType
	{
		Sprite,
		Shape,
		Text
	};

public:
	// constructor, passing in fonts and textures
	Button(State::Context context);
	Button(State::Context context, ButtonType type);

	// set std::function callback
	void setCallback(Callback callback);

	// change button text
	void setText(const std::string& text);

	// button toggle
	void setToggle(bool flag);

	// change type
	void setType(ButtonType type);

	// overide default texture
	void setTexture(const sf::Texture& texture, ButtonState state);

	void setShape(std::unique_ptr<sf::CircleShape> shape);
	void setShapeColor(sf::Color& color, ButtonState state);

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

	void updateApperance(ButtonState state);

	bool checkMouseLocation();

private:
	Callback nCallback;			// std::function
	ButtonType nType;

	// textures, for memaddresses
	sf::Texture& nNormalTexture;			
	sf::Texture& nSelectedTexture;
	sf::Texture& nPressedTexture;

	// sfml members for display
	sf::Sprite nSprite;
	sf::Text nText;
	
	bool nIsToggle;	


	std::unique_ptr<sf::CircleShape> nShape;

	std::shared_ptr<sf::Color> nNormalColor;
	std::shared_ptr<sf::Color> nSelectedColor;
	std::shared_ptr<sf::Color> nPressedColor;

	sf::RenderWindow& nWindow;


	SoundPlayer& nSounds;
};


}


#endif