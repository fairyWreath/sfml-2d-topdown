// loading a texture from a file
template <typename Resource, typename Identifier>
void ResourceHolder<Resource ,Identifier>::load(Identifier id, const std::string& filename)		// template added right after class name
{
	// initialize new pointer for sf::Texture
	std::unique_ptr<Resource> resource = std::make_unique<Resource>();

	if (!resource->loadFromFile(filename))		// return false if fail to load
		throw std::runtime_error("ResourceHolder::load = Failed to load " + filename);				// throw here and caught later

	// move the sf::Texture and insert to resource map, returns iterator to inserted key-value pair and a BOOL
	auto inserted = nResourceMap.insert(std::make_pair(id, std::move(resource)));				// with std::make_pair there is no need to specify the types
	assert(inserted.second);			// second value is a bool value, from type inference
}


// accessing the texture, returning the memory address
template <typename Resource, typename Identifier>
Resource& ResourceHolder<Resource, Identifier>::get(Identifier id) 
{
	auto found = nResourceMap.find(id);			// get iterator from the map
	assert(found != nResourceMap.end());			// assert to enure found iterator does not equal 'null'

	return *found->second;						// get sf::Texture unique_ptr from the map, with * and second to get the value
}

// if we only have a pointer to the const TextureHolder
template <typename Resource, typename Identifier>
const Resource& ResourceHolder<Resource, Identifier>::get(Identifier id) const
{
	auto found = nResourceMap.find(id);			// get iterator from the map
	assert(found != nResourceMap.end());			// assert to enure found iterator does not equal 'null'

	return *found->second;						// get sf::Texture unique_ptr from the map, with * and second to get the value
}


// load method for sf classes that takes extra parameter/s, eg. sf::shader
template <typename Resource, typename Identifier>
template <typename Parameter>
void ResourceHolder<Resource, Identifier>::load(Identifier id, const std::string& filename, const Parameter& secondParam)
{
	std::unique_ptr<Resource> resource = std::make_unique<Resource>();
	if (!resource->loadFromFile(filename, secondParam))		// return false if fail to load, with second param added
		throw std::runtime_error("ResourceHolder::load = Failed to load " + filename);

	auto inserted = nResourceMap.insert(std::make_pair(id, std::move(resource)));
	assert(inserted.second);
}


