#include "ParallelTask.hpp"

ParallelTask::ParallelTask() :
	nThread(&ParallelTask::runTask, this),
	nFinished(false)
{
}

// execute the thread
void ParallelTask::execute()
{
	nFinished = false;			// restart, set finished to false
	nElapsedTime.restart();		// restart fake time

	nThread.launch();			// launch the thread
}

// mutex protects both nClock and nFinished from concurrent access
bool ParallelTask::isFinished()
{
	// mutex works in a 'scope-like' fashion
	sf::Lock lock(nMutex);		// lock mutex
	return nFinished;
}

float ParallelTask::getCompletion()
{
	sf::Lock lock(nMutex);			// lock mutex

	// % based on 10 seconds
	return nElapsedTime.getElapsedTime().asSeconds() / 10.f;
}

// the actual function
void ParallelTask::runTask()
{
	// dummy task, stall 10 seconds
	bool ended = false;
	while (!ended)
	{
		//  protect the lock
		sf::Lock lock(nMutex);
		if (nElapsedTime.getElapsedTime().asSeconds() >= 10.f)	// if already 10 seconds	
			ended = true;
	}

	// protect nFinished as it may be accessed by multiple threads
	{
		sf::Lock lock(nMutex);
		nFinished = ended;
	}
}

