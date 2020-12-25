#ifndef PARTICLE_HPP
#define PARTICLE_HPP

#include <SFML/System/Vector2.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Color.hpp>

// base struct for particles
struct Particle
{
	enum Type
	{
		LightPinkBeam,
		CirclePlasmaBeam,
		BlueBeam,
		PurpleBeam,
		CyanHeartBeam,
		ParticleCount,
	};

	sf::Vector2f position;
	sf::Color color;
	sf::Time lifetime;
};



#endif