#include "EmitterNode.hpp"
#include "ParticleNode.hpp"
#include "CommandQueue.hpp"
#include "Command.hpp"

#include <iostream>

EmitterNode::EmitterNode(Particle::Type type) :
	SceneNode(),
	nType(type),
	nAccumulatedTime(sf::Time::Zero),
	nParticleSystem(nullptr)
{
	//std::cout << "emmiter node created\n";
}


void EmitterNode::updateCurrent(sf::Time dt, CommandQueue& commands)
{
	std::cout << "update current reached\n";
	// emit particles if the particle system has already been initialized
	if (nParticleSystem)
	{
		emitParticles(dt);
	}
	else		// otherwise find the system corresponding to the emitter(same type_
	{
		// create a command to find particlenodes with the same type
		auto finder = [this](ParticleNode& container, sf::Time)
		{
			if (container.getParticleType() == nType)
				nParticleSystem = &container;
		};
		std::cout << "update current reached\n";
		Command command;
		command.category = Category::ParticleSystem;
		command.action = derivedAction<ParticleNode>(finder);

		commands.push(command);
	}
}

/* method to emit particles
- set an emission rate and try to achieve it as closely as possible
- use accumulators to set rate equal to logic frame rate, emit particles as long as the emission
 rate fits into the current frame
 - remaining time stored in nAccumulatedTime and carried over to next frame
 */

void EmitterNode::emitParticles(sf::Time dt)
{
	std::cout << "emit particles called\n";

	const float emissionRate = 30.f;
	const sf::Time interval = sf::seconds(1.f) / emissionRate;

	nAccumulatedTime += dt;

	// if accumulated more than interval
	while (nAccumulatedTime > interval)
	{
		nAccumulatedTime -= interval;	// reset
		nParticleSystem->addParticle(getWorldPosition());		// add the particle
	}
}