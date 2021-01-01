#include "Button.hpp"
#include "Utility.hpp"
#include "SoundPlayer.hpp"
#include "ResourceHolder.hpp"

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include <iostream>

namespace GUI
{

Button::Button(State::Context context, ButtonType type) :
	nCallback(),	
	nIsToggle(false),
	nSounds(*context.soundPlayer),
	nType(type),
	nShape(),
	nNormalColor(),
	nSelectedColor(),
	nPressedColor(),
	nWindow(*context.window)
{
	switch (nType)
	{
	
	case Sprite:
	{
		nNormalTexture = std::make_shared<sf::Texture>(context.textures->get(Textures::MainButtonNormal));
		nSelectedTexture = std::make_shared<sf::Texture>(context.textures->get(Textures::MainButtonSelected));
		nPressedTexture = std::make_shared<sf::Texture>(context.textures->get(Textures::MainButtonPressed));

		nSprite = std::make_unique<sf::Sprite>();
		nSprite->setTexture(*nNormalTexture);

		nText = std::make_unique<sf::Text>("", context.fonts->get(Fonts::Main), 30);
		nText->setFillColor(sf::Color(255, 91, 165));
		sf::FloatRect bounds = nSprite->getLocalBounds();
		nText->setPosition(bounds.width / 2.f, bounds.height / 2.f);
		break;
	}
	
	case Text:
	{
		nText = std::make_unique<sf::Text>("", context.fonts->get(Fonts::Main), 30);
		nText->setFillColor(sf::Color(255, 91, 165));
		break;
	}

	case Shape:
		return;				// handled at setShape


	}
}


// set std::function callback, with std::move
void Button::setCallback(Callback callback)
{
	nCallback = std::move(callback);
}

void Button::setType(ButtonType type)
{
	nType = type;
}


// change text
void Button::setText(const std::string& text)
{
	assert(nType == Sprite || nType == Text);

	nText->setString(text);
	centerOrigin(*nText);
}

void Button::setTexture(const sf::Texture& texture, ButtonState state)
{
	assert(nType == Sprite);

	switch (state)
	{
	case Normal:
		nNormalTexture = std::make_shared<sf::Texture>(texture);
		break;
	case Selected:
		nSelectedTexture = std::make_shared<sf::Texture>(texture);
		break;
	case Pressed:
		nPressedTexture = std::make_shared<sf::Texture>(texture);
		break;
	}
}

// for shapes
void Button::setShape(std::unique_ptr<sf::CircleShape> shape)
{
	assert(nType == Shape);

	nShape = std::move(shape);
	nShape->setPosition(getPosition());
}

void Button::setShapeColor(sf::Color& color, ButtonState state)
{
	assert(nType == Shape);

	switch (state)
	{
	case Normal:
		nNormalColor = std::make_shared<sf::Color>(color);
		nShape->setFillColor(*nNormalColor);
		break;
	case Selected:
		nSelectedColor = std::make_shared<sf::Color>(color);
		break;
	case Pressed:
		nPressedColor = std::make_shared<sf::Color>(color);
		break;
	}
}

// set toggle flag, for button pressing
void Button::setToggle(bool flag)
{
	nIsToggle = flag;
}

// virtual functions from Component class
bool Button::isSelectable() const
{
	return true;		// buttons are selectable
}

// change sprite to selected
void Button::select()
{
	// selection handled in Component class
	Component::select();		

	// play sound
	nSounds.play(SoundEffect::ButtonSelect);

	updateApperance(Selected);
}

void Button::deselect()
{
	Component::deselect();
	
	updateApperance(Normal);

}

// activate and run callback
void Button::activate()
{	
	Component::activate();		// call parent virtual deactivation

	// play the sound
	nSounds.play(SoundEffect::ButtonActivate);

	// if toggled show button is pressed and 'toggled'
	if (nIsToggle)
	{
		updateApperance(Pressed);
	}

	// run the std::function
	if (nCallback)
		nCallback();

	// if not toggled deactivate the button since we are only momentarily activated
	if (!nIsToggle)
		deactivate();
}

// return to normal button state
void Button::deactivate()
{
	Component::deactivate();

	// reset toggle texture
	if (nIsToggle)
	{
		// reset based on if the button is selected or no
		if (isSelected())		// from component class
		{
			updateApperance(Selected);
		}
		else
		{
			updateApperance(Normal);
		}
	}
}


// events handled in activation/callbacks
void Button::handleEvent(const sf::Event& event)
{
	if (checkMouseLocation() && !isSelected())
	{
		select();
	}
	else if (isSelected())
	{
		if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
		{
			activate();
		}
		
		else if (!checkMouseLocation())
		{
			deselect();
		}
	}
}

void Button::updateApperance(ButtonState state)
{
	if (nType == Sprite)
	{
		switch (state)
		{
		case GUI::Button::Normal:
			nSprite->setTexture(*nNormalTexture);
			break;
		case GUI::Button::Selected:
			nSprite->setTexture(*nSelectedTexture);
			break;
		case GUI::Button::Pressed:
			nSprite->setTexture(*nPressedTexture);
			break;
		}
	}
	else
	{
		sf::Color colorToSet;
		switch (state)
		{
		case Normal:
			colorToSet = *nNormalColor;
			break;
		case Selected:
			colorToSet = *nSelectedColor;
			break;
		case Pressed:
			colorToSet = *nPressedColor;
			break;
		}

		if (nType == Shape)
			nShape->setFillColor(colorToSet);
		else if (nType == Text)
			nText->setFillColor(colorToSet);
	}
}


// allow window to draw
void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	// get absolute transform
	states.transform *= getTransform();

	switch (nType)
	{
	case Sprite:
		target.draw(*nSprite, states);
		target.draw(*nText, states);
		break;
	case Text:
		target.draw(*nText, states);
		break;
	case Shape:
		target.draw(*nShape, states);
		break;
	}
}

// check if mouse is within button bounds
bool Button::checkMouseLocation()
{
	sf::Vector2i mousePosition = sf::Mouse::getPosition(nWindow);
	
	sf::FloatRect buttonBounds;

	switch (nType)
	{
	case Sprite:
		buttonBounds = getTransform().transformRect(nSprite->getGlobalBounds());
		break;
	case Shape:
		buttonBounds = getTransform().transformRect(nShape->getGlobalBounds());
		break;
	case Text:
		buttonBounds = getTransform().transformRect(nText->getGlobalBounds());
		break;
	}

	if (buttonBounds.contains(static_cast<sf::Vector2f>(mousePosition)))
	{
		return true;
	}
	else
		return false;
}


}