#ifndef EMITTERNODE_HPP
#define EMITTERNODE_HPP

// base class for emitter nodes

#include "SceneNode.hpp"
#include "Particle.hpp"

// forwad declare ParticleNode class
class ParticleNode;

class EmitterNode : public SceneNode
{
public:
	explicit EmitterNode(Particle::Type type);

private:
	// only override scenenode update
	virtual void updateCurrent(sf::Time dt, CommandQueue& commands);
	void emitParticles(sf::Time dt);

private:
	sf::Time nAccumulatedTime;		// accumulator to set frames equlal to logic frame rate
	Particle::Type nType;

	// pointer into which the emitter node emits particle, initially null
	ParticleNode* nParticleSystem;

};



#endif