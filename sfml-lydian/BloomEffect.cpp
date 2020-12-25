#include "BloomEffect.hpp"

BloomEffect::BloomEffect() :
	nShaders(),
	nBrightnessTexture(),
	nFirstPassTextures(),
	nSecondPassTextures()
{
	// load shaders vertex
	nShaders.load(Shaders::BrightnessPass, "Media/Shaders/Fullpass.vert", "Media/Shaders/Brightness.frag");
	nShaders.load(Shaders::DownSamplePass, "Media/Shaders/Fullpass.vert", "Media/Shaders/DownSample.frag");
	nShaders.load(Shaders::GaussianBlurPass, "Media/Shaders/Fullpass.vert", "Media/Shaders/GuassianBlur.frag");
	nShaders.load(Shaders::AddPass, "Media/Shaders/Fullpass.vert", "Media/Shaders/Add.frag");
}


// main apply logic, override PostEffect method
void BloomEffect::apply(const sf::RenderTexture& input, sf::RenderTarget& output)
{
	// first prepare textures and filter brightness
	prepareTextures(input.getSize());
	filterBright(input, nBrightnessTexture);

	// first blur
	downsample(nBrightnessTexture, nFirstPassTextures[0]);
	blurMultipass(nFirstPassTextures);

	// second blur
	downsample(nFirstPassTextures[0], nSecondPassTextures[0]);
	blurMultipass(nSecondPassTextures);

	
	add(nFirstPassTextures[0], nSecondPassTextures[0], nFirstPassTextures[0]);
	// display firstpass texture
	nFirstPassTextures[1].display();
	
	// add to output
	add(input, nFirstPassTextures[1], output);
}
void BloomEffect::prepareTextures(sf::Vector2u size)
{
	// adapt to size of input
	if (nBrightnessTexture.getSize() != size)
	{
		nBrightnessTexture.create(size.x, size.y);
		nBrightnessTexture.setSmooth(true);

		// first blur, half the size
		nFirstPassTextures[0].create(size.x / 2, size.y / 2);
		nFirstPassTextures[0].setSmooth(true);
		nFirstPassTextures[1].create(size.x / 2, size.y / 2);
		nFirstPassTextures[1].setSmooth(true);

		// second blur, quarter the size
		nSecondPassTextures[0].create(size.x / 4, size.y / 4);
		nSecondPassTextures[0].setSmooth(true);
		nSecondPassTextures[1].create(size.x / 4, size.y / 4);
		nSecondPassTextures[1].setSmooth(true);
	}
}

// sf::shader to get brightness
void BloomEffect::filterBright(const sf::RenderTexture& input, sf::RenderTexture& output)
{
	// get shader from resource holder
	sf::Shader& brightness = nShaders.get(Shaders::BrightnessPass);

	// set parameter and apply
	brightness.setUniform("source", input.getTexture());		// parameters name and Texture, Texture got from rendertexture
	// PostEffect method; apply shader to rendertexture output
	applyShader(brightness, output);
	output.display();		// display the rendertextyre
}


// blur multiple
void BloomEffect::blurMultipass(RenderTextureArray& renderTextures)
{
	sf::Vector2u textureSize = renderTextures[0].getSize();

	for (std::size_t count = 0; count < 2; ++count)
	{
		blur(renderTextures[0], renderTextures[1], sf::Vector2f(0.f, 1.f / textureSize.y));
		blur(renderTextures[1], renderTextures[0], sf::Vector2f(1.f / textureSize.x, 0.f));
	}
}

void BloomEffect::blur(const sf::RenderTexture& input, sf::RenderTexture& output, sf::Vector2f offsetFactor)
{
	sf::Shader& gaussianBlur = nShaders.get(Shaders::GaussianBlurPass);

	gaussianBlur.setUniform("source", input.getTexture());
	gaussianBlur.setUniform("offsetFactor", offsetFactor);
	applyShader(gaussianBlur, output);
	output.display();
}

void BloomEffect::downsample(const sf::RenderTexture& input, sf::RenderTexture& output)
{
	sf::Shader& downSampler = nShaders.get(Shaders::DownSamplePass);

	downSampler.setUniform("source", input.getTexture());
	downSampler.setUniform("sourceSize", sf::Vector2f(input.getSize()));
	applyShader(downSampler, output);
	output.display();
}

void BloomEffect::add(const sf::RenderTexture& source, const sf::RenderTexture& bloom, sf::RenderTarget& output)
{
	sf::Shader& adder = nShaders.get(Shaders::AddPass);

	adder.setUniform("source", source.getTexture());
	adder.setUniform("bloom", bloom.getTexture());
	applyShader(adder, output);
}


