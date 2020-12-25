#ifndef BLOOMEFFECT_HPP
#define BLOOMEFFECT_HPP

#include "PostEffect.hpp"
#include "ResourceIdentifiers.hpp"
#include "ResourceHolder.hpp"

#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Shader.hpp>

#include <array>

class BloomEffect : public PostEffect
{
public:
	BloomEffect();

	// override Post EFfect apply
	virtual void apply(const sf::RenderTexture& input, sf::RenderTarget& output);

private:
	// rendertexture with '2' layers, for firstpasstextures and secondpasstexturres
	typedef std::array<sf::RenderTexture, 2> RenderTextureArray;

private:
	void prepareTextures(sf::Vector2u size);

	/* main steps in creating the bloom effect */
	// filter bright colors
	void filterBright(const sf::RenderTexture& input, sf::RenderTexture& output);

	// blur twice
	void blurMultipass(RenderTextureArray& renderTextures);
	void blur(const sf::RenderTexture& input, sf::RenderTexture& output, sf::Vector2f offsetFactor);

	// downsample and add
	void downsample(const sf::RenderTexture& input, sf::RenderTexture& output);
	void add(const sf::RenderTexture& source, const sf::RenderTexture& bloom, sf::RenderTarget& target);


private:
	ShaderHolder nShaders;

	// temporary rendertextures in processing
	sf::RenderTexture nBrightnessTexture;
	RenderTextureArray nFirstPassTextures;
	RenderTextureArray nSecondPassTextures;

};



#endif