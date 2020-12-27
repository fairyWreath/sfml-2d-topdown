#include "Button.hpp"
#include "Utility.hpp"
#include "SoundPlayer.hpp"
#include "ResourceHolder.hpp"

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

namespace GUI
{

Button::Button(State::Context context) :
	nCallback(),		// set callback to null
	// initialize button textures from texture holder
	nNormalTexture(context.textures->get(Textures::MainButtonNormal)),
	nSelectedTexture(context.textures->get(Textures::MainButtonSelected)),
	nPressedTexture(context.textures->get(Textures::MainButtonPressed)),
	nSprite(),
	nText("", context.fonts->get(Fonts::Main), 26),		// set to empty string
	nIsToggle(false),
	nSounds(*context.soundPlayer)
{
	// set intial(normal) texture
	nSprite.setTexture(nNormalTexture);

	// change text color
	nText.setFillColor(sf::Color::Magenta);

	// set position of text based on sprite bounds
	sf::FloatRect bounds = nSprite.getLocalBounds();
	nText.setPosition(bounds.width / 2.f, bounds.height / 2.f);
}

// set std::function callback, with std::move
void Button::setCallback(Callback callback)
{
	nCallback = std::move(callback);
}

// change text
void Button::setText(const std::string& text)
{
	nText.setString(text);
	centerOrigin(nText);
}

void Button::setTexture(const sf::Texture& texture, ButtonState state)
{
	switch (state)
	{
	case Normal:
		nNormalTexture = texture;
		break;
	case Selected:
		nSelectedTexture = texture;
		break;
	case Pressed:
		nPressedTexture = texture;
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

	// change sprite to selected
	nSprite.setTexture(nSelectedTexture);
}

void Button::deselect()
{
	// deselection handled in Component class
	Component::deselect();

	// change sprite to normal
	nSprite.setTexture(nNormalTexture);
}

// activate and run callback
void Button::activate()
{	
	Component::activate();		// call parent virtual deactivation

	// play the sound
	nSounds.play(SoundEffect::ButtonActivate);

	// if toggled show button is pressed and 'toggled'
	if (nIsToggle)
		nSprite.setTexture(nPressedTexture);

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
			nSprite.setTexture(nSelectedTexture);
		else
			nSprite.setTexture(nNormalTexture);
	}
}


// events handled in activation/callbacks
void Button::handleEvent(const sf::Event&)
{
}

// allow window to draw
void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	// get absolute transform
	states.transform *= getTransform();

	// draw text and sprite
	target.draw(nSprite, states);
	target.draw(nText, states);
}


}