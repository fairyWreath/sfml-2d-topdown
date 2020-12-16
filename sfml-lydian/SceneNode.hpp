#ifndef SCENENODE_HPP
#define SCENENODE_HPP

// node class for tree graph, for managing transforms

#include <SFML/System/NonCopyable.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Drawable.hpp>

#include <memory>
#include <vector>
#include <cassert>


// scene layers
enum Layer
{
	Background,
	Void,
	Layercount


};


/*	SceneNode inherits from
1. sf::Drawable so they can be drawn
2. sf::Transformable for rotaion, position, scaling
3. sf::NonCopyable to state that scene nodes cannot be copied(with copy constuctor etc.)
*/

class SceneNode : public sf::Transformable, sf::Drawable, sf::NonCopyable
{
public:
	typedef std::unique_ptr<SceneNode> Ptr;			// rename unique_ptr to Ptr

public:
	SceneNode();

	// node insertion
	void attachChild(Ptr child);


	// node removal
	Ptr detachChild(const SceneNode& node);

	// updating the scene
	


// virtual drawing methods, inherited from sf classes
private:
	/*
	the sf::Window class can INTERNALLY call these draw functions
	*/

	/* functions have to deal with two different types of trnansforms:
	1. current node's transform that determines position, rotation, scale relative to parent node
	2. parameter states, which is passed by value and has a member transform of sf::Transform.
	   this contains information of where to render the parent scene node
	*/

	// draw on current node and all children nodes
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	
	// draw on current node only, to be used with draw function 
	virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;

private:

	// children only store their position, scale and rotation relative to their parent
	std::vector<Ptr> nChildren;				// use unique_ptr<SceneNode> not SceneNode as std::vector has to be a completed type
	SceneNode* nParent;						// pointer to parent node
};


#endif