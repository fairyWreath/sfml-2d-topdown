#include "ResourceHolder.hpp"

#include <SFML/Graphics.hpp>

#include <iostream>

// Resource Identifier for textures
namespace Textures
{
	enum Id
	{
		Landscape,
		Character,
	};
}

int main()
{
	sf::RenderWindow window(sf::VideoMode(640, 480), "Lydian");

	/* loading the resources */
	// load from file
	ResourceHolder<sf::Texture, Textures::Id> textures;				// declare textures class, template types provided after class name
	try
	{
		textures.load(Textures::Character, "Media/Textures/Magician-Girl-Down.png");
		textures.load(Textures::Landscape, "Media/Textures/Void.png");
	}
	catch (std::runtime_error& e)		// catch error address here
	{
		std::cout << "Exception: " << e.what() << std::endl;
		return 1;
	}

	// accessing the resources
	sf::Sprite character(textures.get(Textures::Character));		// pass in sf::Texture& (memory address)
	sf::Sprite landscape(textures.get(Textures::Landscape));
	character.setPosition(200.f, 200.f);

	// render to window
	while (window.isOpen())
	{
		sf::Event event;			// handle events
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::KeyPressed || event.type == sf::Event::Closed)				// break if key is pressed or window x is closed
				return 0;
		}

		window.clear();

		// draw sprites here
		window.draw(landscape);
		window.draw(character);		
		window.display();				// display/update changes
	}
}

