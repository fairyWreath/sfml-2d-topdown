#include "SoundPlayer.hpp"

#include <SFML/Audio/Listener.hpp>

#include <cmath>

namespace
{
	// coordinate system of the sound, point of view of player in front of the screen
	const float ListenerZ = 300.f;
	const float Attenuation = 8.f;
	const float MinDistance2D = 200.f;
	const float MinDistance3D = std::sqrt(MinDistance2D * MinDistance2D + ListenerZ * ListenerZ);
}


SoundPlayer::SoundPlayer() :
	nSoundBuffers(),
	nSounds(),
	nVolume(60.f)
{
	// loading from the files
	nSoundBuffers.load(SoundEffect::ButtonSelect, "Media/Sound/ButtonSelect.wav");
	nSoundBuffers.load(SoundEffect::ButtonActivate, "Media/Sound/ButtonActivate.wav");
	nSoundBuffers.load(SoundEffect::Explosion1, "Media/Sound/Explosion1.wav");
	nSoundBuffers.load(SoundEffect::Explosion2, "Media/Sound/Explosion2.wav");

	nSoundBuffers.load(SoundEffect::AlliedSingleAttack, "Media/Sound/LaserAttack.wav");
	nSoundBuffers.load(SoundEffect::AlliedCircularAttack, "Media/Sound/FreezeAttack.wav");
	nSoundBuffers.load(SoundEffect::EnemyAttack, "Media/Sound/LaserAttack.wav");

	nSoundBuffers.load(SoundEffect::SpecialAttack, "Media/Sound/LaserAttack.wav");


	// set listener to point towards the screen
	sf::Listener::setDirection(0.f, 0.f, -1.f);
}


// play the sound; add to list and play the last element
void SoundPlayer::play(SoundEffect::ID sound)
{
	// get soundbuffer and create sf::Sound from it
	//nSounds.push_back(sf::Sound(nSoundBuffers.get(sound)));
	//nSounds.back().play();				// play the newly inserted sound

	// play with current position
	play(sound, getListenerPosition());
}


// play with given position
void SoundPlayer::play(SoundEffect::ID effect, sf::Vector2f position)
{
	// push to list, and access it later and modify it with given position
	nSounds.push_back(sf::Sound());
	sf::Sound& sound = nSounds.back();

	sound.setBuffer(nSoundBuffers.get(effect));
	sound.setPosition(position.x, -position.y, 0.f);
	sound.setAttenuation(Attenuation);
	sound.setMinDistance(MinDistance3D);
	sound.setVolume(nVolume);

	sound.play();
}


// remove stopped sounds from list
void SoundPlayer::removeStoppedSounds()
{
	// remove if, accepts list element type as a parameter with bool return type
	nSounds.remove_if([](const sf::Sound& sound) {
		return sound.getStatus() == sf::Sound::Stopped;
		});
}



// setting and getting listener positions
void SoundPlayer::setListenerPosition(sf::Vector2f position)
{
	sf::Listener::setPosition(position.x, -position.y, ListenerZ);
}

sf::Vector2f SoundPlayer::getListenerPosition() const
{
	sf::Vector3f position = sf::Listener::getPosition();
	return sf::Vector2f(position.x, -position.y);
}

void SoundPlayer::modifyVolume(float amount)
{
	if ((nVolume + amount) >= 0.f && (nVolume + amount) <= 100.f)
		nVolume += amount;
}

float SoundPlayer::getVolume() const
{
	return nVolume;
}