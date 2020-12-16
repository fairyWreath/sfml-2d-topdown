#ifndef RESOURCEIDENTIFIERS_HPP
#define RESOURCEIDENTIFIERS_HPP

// header file to store resource 'types'

// forward declaration of SFML classes, to be used in typedef below
namespace sf
{
	class Texture;
}

namespace Textures
{
	enum ID
	{
		DarkMagician,
		Void,
	};
}

// forward declaration of template class
template <typename Resource, typename Identifier>
class ResourceHolder;

typedef ResourceHolder<sf::Texture, Textures::ID> TextureHolder;			// used forward declarations

#endif