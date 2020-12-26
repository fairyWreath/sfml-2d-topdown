#ifndef MUSICPLAYER_HPP
#define MUSICPLAYER_HPP

#include "ResourceHolder.hpp"
#include "ResourceIdentifiers.hpp"

#include <SFML/System/NonCopyable.hpp>
#include <SFML/Audio/Music.hpp>


#include <map>
#include <string>


class MusicPlayer : private sf::NonCopyable
{
public:
	MusicPlayer();

	void play(Music::ID theme);
	void stop();

	void setPaused(bool paused);
	void setVolume(float volume);

	bool isMusicPlaying() const;

	sf::Time getCurrentMusicOffset() const;
	void setCurrentMusicOffset(sf::Time time);

	Music::ID getCurrentMusic() const;

private:

	// using sf::Music to represent currently played music
	sf::Music nMusic;
	Music::ID nCurrentMusic;

	// map music id to file names
	std::map<Music::ID, std::string> nFileNames;
	
	float nVolume;
};


#endif