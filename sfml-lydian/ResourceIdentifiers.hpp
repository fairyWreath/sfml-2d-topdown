#ifndef RESOURCEIDENTIFIERS_HPP
#define RESOURCEIDENTIFIERS_HPP

// header file to store resource 'types'

// forward declaration of SFML classes, to be used in typedef below
namespace sf
{
	class Texture;
	class Font;
}


// namespaces for resources, ie. textures/fonts
namespace Textures
{
	enum ID
	{
		TitleScreen,
		MenuScreen,
		PauseScreen,
		SettingsScreen,

		// characters
		DarkMagician,
		Shinobu,
		Izuko,
		Hitagi,

		Void,

		// projectiles
		AlliedNormal,			// normal flower projectiles
		EnemyNormal,
		SpecialHeart,

		// buttons
		MainButtonNormal,
		MainButtonSelected,
		MainButtonPressed,

		// powerups
		HealthRefill,
		SpecialAttackRefill,
		AttackSpread,
		AttackRate,
		MovementSpeed
	};
}

namespace Fonts
{
	enum ID
	{
		Title,		//denalova
		Main,
		Label
	};
}


// forward declaration of template class
template <typename Resource, typename Identifier>
class ResourceHolder;

// forward declarations used here
typedef ResourceHolder<sf::Texture, Textures::ID> TextureHolder;			// typedef for texture type

// typedef for font resource holder
typedef ResourceHolder<sf::Font, Fonts::ID> FontHolder;

#endif