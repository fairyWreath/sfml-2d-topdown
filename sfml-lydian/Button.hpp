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
	Button(State::Context context, ButtonType type = Sprite);

	void setCallback(Callback callback);
	void setText(const std::string& text);
	void setToggle(bool flag);
	void setType(ButtonType type);

	void setTexture(const sf::Texture& texture, ButtonState state);
	void setShape(std::unique_ptr<sf::CircleShape> shape);
	void setShapeColor(sf::Color& color, ButtonState state);
	virtual bool isSelectable() const;	

	virtual void select();
	virtual void deselect();
	virtual void activate();
	virtual void deactivate();

	virtual void handleEvent(const sf::Event& event);

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	void updateApperance(ButtonState state);

	bool checkMouseLocation();

private:
	Callback nCallback;			
	ButtonType nType;

	std::shared_ptr<sf::Texture> nNormalTexture;
	std::shared_ptr<sf::Texture> nSelectedTexture;
	std::shared_ptr<sf::Texture> nPressedTexture;

	
	std::unique_ptr<sf::Sprite> nSprite;
	std::unique_ptr<sf::Text> nText;

	std::unique_ptr<sf::CircleShape> nShape;

	std::shared_ptr<sf::Color> nNormalColor;
	std::shared_ptr<sf::Color> nSelectedColor;
	std::shared_ptr<sf::Color> nPressedColor;

	bool nIsToggle;
	sf::RenderWindow& nWindow;

	SoundPlayer& nSounds;
};


}


#endif