#ifndef CHARACTER_HPP
#define CHARACTER_HPP

// character class that inherits from Entity

#include "Entity.hpp"


class Character : public Entity
{
public:
	// enum to store character types
	enum Type
	{
		DarkMagician,
	};

public:
	// explicit constructor
	explicit Character(Type type);

private:
	Type nType;

};


#endif