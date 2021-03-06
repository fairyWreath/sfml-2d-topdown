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
#include <set>					// python set()

#include "Category.hpp"			// entitiy categories to dispatch commands

// forward declare commands
struct Command;
class CommandQueue;

/*	SceneNode inherits from
1. sf::Drawable so they can be drawn
2. sf::Transformable for rotaion, position, scaling
3. sf::NonCopyable to state that scene nodes cannot be copied(with copy constuctor etc.)
*/
//class SceneNode : public sf::Transformable, public sf::Drawable, private sf::NonCopyable

class SceneNode : public sf::Transformable, public sf::Drawable, private sf::NonCopyable
{
public:
	typedef std::unique_ptr<SceneNode> Ptr;			// rename unique_ptr to Ptr

	// collision pairs between two scenenodes
	typedef std::pair<SceneNode*, SceneNode*> Pair;

public:
	explicit SceneNode(Category::Type category = Category::None);

	// node insertion
	void attachChild(Ptr child);


	// node removal
	Ptr detachChild(const SceneNode& node);

	// updating the scene
	void update(sf::Time dt, CommandQueue& commands);		// accept tick time as parameter


	// absolute transforms
	sf::Vector2f getWorldPosition() const;
	sf::Transform getWorldTransform() const;


	// get category for command, virtual function so it can be overidden by other classes
	virtual unsigned int getCategory() const;


	// run every time a command is passed to a scenenode, a non virtual function
	// use bitwise & operator to the check that the command's receiver is the same category
	void onCommand(const Command& command, sf::Time dt);

	/* for collisions */
	void checkSceneCollision(SceneNode& sceneGraph, std::set<Pair>& collisionPairs);

	// check between *this and children
	void checkNodeCollision(SceneNode& node, std::set<Pair>& collisionPairs);

	// get bouding floatrect, for collisions
	virtual sf::FloatRect getBoundingRect() const;


	// remove dangling wrecks
	void removeWrecks();

	virtual bool isMarkedForRemoval() const;
	virtual bool isDestroyed() const;


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
	// draw on node children
	void drawChildren(sf::RenderTarget& target, sf::RenderStates states) const;


	// updating the scene
	virtual void updateCurrent(sf::Time dt, CommandQueue& commands);
	void updateChildren(sf::Time dt, CommandQueue& commands);


private:

	// children only store their position, scale and rotation relative to their parent
	std::vector<Ptr> nChildren;				// use unique_ptr<SceneNode> not SceneNode as std::vector has to be a completed type
	SceneNode* nParent;						// pointer to parent node

	// scenenode category
	Category::Type nDefaultCategory;
};

// functions go get collision and distance between two scene nodes
bool collision(const SceneNode& left, const SceneNode& right);		// check if two nodes collide
float distance(const SceneNode& left, const SceneNode& right);


#endif