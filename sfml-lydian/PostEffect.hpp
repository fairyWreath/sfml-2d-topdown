#ifndef POSTEFFECT_HPP
#define POSTEFFECT_HPP

//abstractclass for post effects(whole screen)

#include <SFML/System/NonCopyable.hpp>

namespace sf
{
	class RenderTarget;
	class RenderTexture;
	class Shader;
}

class PostEffect : sf::NonCopyable
{
public:
	virtual ~PostEffect();

	// apply to render texture and target
	virtual void apply(const sf::RenderTexture& input, sf::RenderTarget& output) = 0;

	// check if graphics card supports posts effects, only an alias for sf::Shader::isAvailable
	static bool isSupported();

protected:

	// appying the shader
	static void applyShader(const sf::Shader& shader, sf::RenderTarget& output);

};



#endif