#include "MusicPlayer.hpp"

MusicPlayer::MusicPlayer()	:
	nMusic(),
	nFileNames(),
	nCurrentMusic(Music::None),
	nVolume(100.f)		// set to 100 initially
{
	// mapping the music files 
	nFileNames[Music::MainTheme] = "Media/Music/MainTheme.ogg";
	nFileNames[Music::SecondaryTheme] = "Media/Music/SecondaryTheme.ogg";
	nFileNames[Music::MissionTheme] = "Media/Music/MissionTheme.ogg";
}

void MusicPlayer::play(Music::ID theme)
{
	// get filename
	std::string filename = nFileNames[theme];

	if (!nMusic.openFromFile(filename))
		throw std::runtime_error("Music: " + filename + " could not be loaded.");

	// set volume and play methods
	nMusic.setVolume(nVolume);
	nMusic.setLoop(true);
	nMusic.play();

	nCurrentMusic = theme;
}

void MusicPlayer::stop()
{
	nMusic.stop();
	nCurrentMusic = Music::None;
}

void MusicPlayer::setPaused(bool paused)
{
	if (paused)
		nMusic.pause();
	else
		nMusic.play();
}


void MusicPlayer::setVolume(float volume)
{
	nVolume = volume;
}

bool MusicPlayer::isMusicPlaying() const
{
	sf::Music::Status status = nMusic.getStatus();
	
	if (status == sf::Music::Status::Playing)
		return true;
	else
		return false;
}

Music::ID MusicPlayer::getCurrentMusic() const
{
	return nCurrentMusic;
}

// setting offsets, for the pause state

sf::Time MusicPlayer::getCurrentMusicOffset() const
{
	return nMusic.getPlayingOffset();
}

void MusicPlayer::setCurrentMusicOffset(sf::Time offset)
{
	nMusic.setPlayingOffset(offset);
}