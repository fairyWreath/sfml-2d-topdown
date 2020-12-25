#include "PostEffect.hpp"

#include <SFML/Graphics/Shader.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/VertexArray.hpp>

PostEffect::~PostEffect()
{

}

bool PostEffect::isSupported()
{
	return sf::Shader::isAvailable();
}

// rendering using a shader
void PostEffect::applyShader(const sf::Shader& shader, sf::RenderTarget& output)
{
	// set up a quad using two triangles
	// the quad covers the entire target output

	// get output size from the render target
	sf::Vector2f targetSize = static_cast<sf::Vector2f>(output.getSize());

	// set up the quad here
	sf::VertexArray vertices(sf::TrianglesStrip, 4);		// get vertexc array
	// set up the quad vertices, to the edges of the output target
	vertices[0] = sf::Vertex(sf::Vector2f(0, 0), sf::Vector2f(0, 1));
	vertices[1] = sf::Vertex(sf::Vector2f(targetSize.x, 0), sf::Vector2f(1, 1));
	vertices[2] = sf::Vertex(sf::Vector2f(0, targetSize.y), sf::Vector2f(0, 0));
	vertices[3] = sf::Vertex(sf::Vector2f(targetSize), sf::Vector2f(1, 0));

	// define a states class to convey settings to the output.draw() call
	// in particular, the shader and blendmode 
	sf::RenderStates states;
	states.shader = &shader;
	states.blendMode = sf::BlendNone;

	output.draw(vertices, states);

}

