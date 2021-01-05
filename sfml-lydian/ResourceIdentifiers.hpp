#ifndef RESOURCEIDENTIFIERS_HPP
#define RESOURCEIDENTIFIERS_HPP

// header file to store resource 'types'

// forward declaration of SFML classes, to be used in typedef below
namespace sf
{
	class Texture;
	class Font;
	class Shader;
	class SoundBuffer;
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
		GameEndScreen,

		// characters
		DarkMagician,
		Shinobu,
		Izuko,
		Hitagi,
		Yotsugi,

		Void,

		// projectiles
		AlliedNormal,			// normal flower projectiles
		EnemyNormal,
		AlliedSingle,		// normal attacks

		AlliedSingleBurst,	
		AlliedSingleQuick,

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
		MovementSpeed,

		// particles
		LightPinkBeam,
		CirclePlasmaBeam,
		BlueBeam,
		PurpleBeam,
		CyanHeartBeam,

		// animations
		ExplosionAnimation,

		// texture atlases
		//Entities,
		//TitleScreen,
		//Void,
		//Buttons,
		Explosion,
		Particle,
		EndLine,


		// character animation sheets
		Elesa,


		// tilesets / tilesheets
		Outside1,
		Outside2,

		Outdoor,

		Fullset1,
		Fullset2,
		Fullset3,
		Fullset4,
		Fullset5,
		Fullset6,
		Fullset7,


		GameRoomBig,

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


namespace Shaders
{
	enum ID
	{
		BrightnessPass,
		DownSamplePass,
		GaussianBlurPass,
		AddPass
	};
}

// music
namespace Music
{
	enum ID
	{
		None,
		MainTheme,
		SecondaryTheme,
		MissionTheme,
	};
}

namespace SoundEffect
{
	enum ID
	{
		AlliedSingleAttack,
		AlliedCircularAttack,
		EnemyAttack,
		Explosion1,
		Explosion2,
		SpecialAttack,
		CollectPowerup,
		ButtonSelect,
		ButtonActivate,
	};
}


// forward declaration of template class
template <typename Resource, typename Identifier>
class ResourceHolder;

typedef ResourceHolder<sf::SoundBuffer, SoundEffect::ID> SoundBufferHolder;


// forward declarations used here
typedef ResourceHolder<sf::Texture, Textures::ID> TextureHolder;			// typedef for texture type

// typedef for font resource holder
typedef ResourceHolder<sf::Font, Fonts::ID> FontHolder;

// for shader / bloom effect
typedef ResourceHolder<sf::Shader, Shaders::ID> ShaderHolder;

#endif