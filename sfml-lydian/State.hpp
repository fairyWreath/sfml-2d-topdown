#ifndef STATE_HPP
#define STATE_HPP

// base class for states

#include "ResourceIdentifiers.hpp"
#include "StateIdentifiers.hpp"
#include "MusicPlayer.hpp"
#include "SoundPlayer.hpp"
#include "TileMap.hpp"

#include <SFML/System/Time.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/View.hpp>

#include <memory>


// forward declaration of classes
namespace sf
{
	class RenderWindow;
}

class StateStack;			// forward declare here, header for statestack included in state.cpp
class Player;
class World;

class State
{
public:
	typedef std::unique_ptr<State> Ptr;			// typedef for state unique ptr

	/* state context structure that works as a HOLDER of shared objects between all states
	-> removes the need to load the same sprite, font etc.
	*/
	struct Context						
	{
		// constructor that receives resources, window and player class
		Context(sf::RenderWindow& window, TextureHolder& textures, FontHolder& fonts, Player& player, 
			MusicPlayer& music, SoundPlayer& sound);

		Context(sf::RenderWindow& window, TextureHolder& textures, FontHolder& fonts, Player& player,
			MusicPlayer& music, SoundPlayer& sound, World& world);
	
		// pointers that hold shared objects above
		sf::RenderWindow* window;
		TextureHolder* textures;
		FontHolder* fonts;
		Player* player;
		MusicPlayer* musicPlayer;
		SoundPlayer* soundPlayer;
		World* world;
	};

public:
	// constructor that accepts the STATESTACK with the context
	State(StateStack& stack, Context context);

	// virtual destructor
	virtual ~State();

	// virtual screen displaying functions, to be used in Application
	virtual void draw() = 0;			// set to 'null'
	virtual bool update(sf::Time dt) = 0;
	virtual bool handleEvent(const sf::Event& event) = 0;
//	virtual bool handleRealTimeInput(sf::Time dt) = 0;

protected:
	// functions for stack operations
	void requestStackPush(States::ID stateID);
	void requestStackPop();
	void requestStateClear();

	// get state context
	Context getContext() const;

private:
	StateStack* nStack;			// pointer to state stack
	Context nContext;			// state context

};



#endif