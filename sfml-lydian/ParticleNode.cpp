#include "ParticleNode.hpp"
#include "DataTables.hpp"
#include "ResourceHolder.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Texture.hpp>

#include <iostream>

// unnameed namespace make unit local
namespace
{
	const std::vector<ParticleData> ParticleTable = initializeParticleData();

	Textures::ID toTextureID(Particle::Type type)
	{
		switch (type)
		{
		case Particle::LightPinkBeam:
			return Textures::LightPinkBeam;
			break;
		case Particle::CirclePlasmaBeam:
			return Textures::CirclePlasmaBeam;
			break;
		case Particle::BlueBeam:
			return Textures::BlueBeam;
			break;
		case Particle::PurpleBeam:
			return Textures::PurpleBeam;
			break;
		case Particle::CyanHeartBeam:
			return Textures::CyanHeartBeam;
			break;
		}
	}
}

ParticleNode::ParticleNode(Particle::Type type, const TextureHolder& textures) :
	SceneNode(),
	nParticles(),
	nTexture(textures.get(Textures::CyanHeartBeam)),
	nType(type),
	nVertexArray(sf::Quads),			// sf::Quads to initialize empty vertex
	nNeedsVertexUpdate(true)
{
}

// add particle to particles deque
void ParticleNode::addParticle(sf::Vector2f position)
{
	Particle particle;
	// get values from the table
	particle.position = position;
	particle.color = ParticleTable[nType].color;
	particle.lifetime = ParticleTable[nType].lifetime;

	nParticles.push_back(particle);
}

/* update method
1. remove all particles that have their lifetime expired
2. all particles have the SAME lifetime(as their types are the same)
older particles are stored at the begining of the container
only check the front of the queue and pop FROM the FRONT if their lifetime is up
-> deque is used exactly for this
4. decrease lifetime of all particles by  reamining current frame time
*/

void ParticleNode::updateCurrent(sf::Time dt, CommandQueue&)
{
	// pop expired particles from the front
	while (!nParticles.empty() && nParticles.front().lifetime <= sf::Time::Zero)
		nParticles.pop_front();

	// decrease lifetime of remaining particles
	for (auto& particle : nParticles)
	{
		particle.lifetime -= dt;
		// std::cout << "particle lifetime decreased\n";
	}
	nNeedsVertexUpdate = true;
}

void ParticleNode::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	// only rebuild particles if something has changed
	if (nNeedsVertexUpdate)
	{
		computeVertices();				// rebuild particles
		nNeedsVertexUpdate = false;
	}
	states.texture = &nTexture;		// change state texture
	target.draw(nVertexArray, states);			// draw using the vertex array
}

// rebuild particles, compute their vertices
void ParticleNode::computeVertices() const
{
	// get texture size
	sf::Vector2f size(nTexture.getSize());
	sf::Vector2f half = size / 2.f;

	nVertexArray.clear();

	// traverse through the deque, compute the 4 vertices for each
	for (const auto& particle : nParticles)
	{
		sf::Vector2f position = particle.position;
		sf::Color color = particle.color;

		// calculate ratio between reamaining and total lifetime, [0, 1] to [0, 255]
		float ratio = particle.lifetime.asSeconds() / ParticleTable[nType].lifetime.asSeconds();
		// particle alpha value [0, 255] to determine transparency, fade out until invisible
		// use sfml sf::Color.a to determine transparency, sf::Uint8 type, with 0 as lowest
		color.a = static_cast<sf::Uint8>(255 * std::max(ratio, 0.5f));

		// add four vertices, one in every corner of the rectangle
		// first two arguments are target coordinates, the second to are texture coordinates
		addVertex(position.x - half.x, position.y - half.y, 0.f, 0.f, color);		// top left
		addVertex(position.x + half.x, position.y - half.y, size.x, 0.f, color);	// top right
		addVertex(position.x + half.x, position.y + half.y, size.x, size.y, color);	// bottom right
		addVertex(position.x - half.x, position.y + half.y, 0.f, size.y, color);	// bottom left
	}
}

// adding a vertex to the array
void ParticleNode::addVertex(float worldX, float worldY, float textCoordX,
	float textCoordY, const sf::Color& color) const
{
	// create vertex type
	sf::Vertex vertex;
	vertex.position = sf::Vector2f(worldX, worldY);
	vertex.texCoords = sf::Vector2f(textCoordX, textCoordY);
	vertex.color = color;

	// add to the array
	nVertexArray.append(vertex);
}


Particle::Type ParticleNode::getParticleType() const
{
	return nType;
}

unsigned int ParticleNode::getCategory() const
{
	return Category::ParticleSystem;
}

