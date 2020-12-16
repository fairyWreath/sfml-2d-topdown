#ifndef RESOURCEHOLDER_HPP
#define RESOURCEHOLDER_HPP


#include <map>
#include <memory>
#include <stdexcept>
#include <cassert>
#include <string>

/*	generalizing resource management
Resource: type of resource, eg. sf::Texture
Identifier: ID type, eg. Textures::ID enum, or std::string
*/

template<typename Resource, typename Identifier>
class ResourceHolder
{
public:
	// load a resource from a file
	void load(Identifier id, const std::string& filename);


	// overloading load for other sf types with extra parameter, for eg. sf::Shader
	template <typename Parameter>
	void load(Identifier id, const std::string& filename, const Parameter& secondParam);

	// accessing the texture, returning the memory address
	Resource& get(Identifier id);
	// if we only have a pointer to the const TextureHolder
	const Resource& get(Identifier id) const;

private:
	// std::map is an STL dict-like container, with keys and values
	// wrap Resource in unique_ptr
	std::map<Identifier, std::unique_ptr<Resource>> nResourceMap;
};

#include "ResourceHolder.inl"
#endif