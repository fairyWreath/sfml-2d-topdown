#ifndef SOUNDPLAYER_HPP
#define SOUNDPLAYER_HPP

#include "ResourceHolder.hpp"
#include "ResourceIdentifiers.hpp"

#include <SFML/System/Vector2.hpp>
#include <SFML/System/NonCopyable.hpp>
#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>

#include <list>


class SoundPlayer : private sf::NonCopyable
{
public:
	SoundPlayer();

	// playing the sounds
	void play(SoundEffect::ID effect);
	void play(SoundEffect::ID effect, sf::Vector2f position);

	// modifying the sounds
	void removeStoppedSounds();
	void setListenerPosition(sf::Vector2f position);
	sf::Vector2f getListenerPosition() const;

	void modifyVolume(float amount);
	float getVolume() const;


private:
	// resource holder of the sounds
	SoundBufferHolder nSoundBuffers;

	float nVolume;

	// currently playing sounds
	std::list<sf::Sound> nSounds;

};


#endif