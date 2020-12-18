#ifndef PARALLELTASK_HPP
#define PARALLELTASK_HPP

// class for threading, eg. loading screens

/* sf::Thread
- link sf::Trhead object to a functio on its construcotr, eg. sf::Thread(&myFunct)
- sf::Thread::launch() to call the linked functio in a separate thread
*/

/* sf::Mutex and sf::Lock for concurrency
- protect shared data when multi-threading
*/

#include <SFML/System/Thread.hpp>
#include <SFML/System/Mutex.hpp>	// concurrency data sharing
#include <SFML/System/Lock.hpp>
#include <SFML/System/Clock.hpp>


class ParallelTask
{
public:
	ParallelTask();

	// function to launch thread
	void execute();
	bool isFinished();

	float getCompletion();

private:
	void runTask();			// main function, to be attached to the thread

private:
	// sfml members for threading
	sf::Thread nThread;	// attach runTask - nThread(&runTask, this)
	bool nFinished;		// track state

	// concurrency - protecting shared data
	sf::Clock nElapsedTime;
	sf::Mutex nMutex;

};


#endif