#ifndef PARTICLENODE_HPP
#define PARTICLENODE_HPP

// base class for particle nodes in the scene graph

#include "SceneNode.hpp"
#include "Particle.hpp"
#include "ResourceIdentifiers.hpp"

#include <SFML/Graphics/VertexArray.hpp>

#include <deque>

class ParticleNode : public SceneNode
{
public:
	ParticleNode(Particle::Type type, const TextureHolder& textures);

	void addParticle(sf::Vector2f position);

	Particle::Type getParticleType() const;

	// override get category
	virtual unsigned int getCategory() const;

private:
	// override update and draw function
	virtual void updateCurrent(sf::Time dt, CommandQueue& commands);
	virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;


	// add to sf::Vertex type
	void addVertex(float worldX, float worldY, float textCoordX, float textCoordY, const sf::Color& color) const;
	
	// rebbuild particles
	void computeVertices() const;

private:
	std::deque<Particle> nParticles;
	const sf::Texture& nTexture;			// texture for the particle
	Particle::Type nType;

	// vertex array
	// MUTABLE here because drawCurrent is a CONST 
	// nVertexArray is mutable because it is not part of the object's logical state
	mutable sf::VertexArray nVertexArray;
	mutable bool nNeedsVertexUpdate;

};


#endif