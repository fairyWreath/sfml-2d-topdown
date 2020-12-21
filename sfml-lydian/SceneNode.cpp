#include "SceneNode.hpp"
#include "Command.hpp"
#include "CommandQueue.hpp"
#include "Utility.hpp"

#include <algorithm>

#include <iostream>

SceneNode::SceneNode() : 
	nChildren(),			// 	 leave children empty
	nParent(nullptr)		// constructor initialize parent pointer to null
{
}

// node insertion
void SceneNode::attachChild(Ptr child)
{
	child->nParent = this;				// set parent to current node
	nChildren.push_back(std::move(child));		// move and attach child
}


// node removal
SceneNode::Ptr SceneNode::detachChild(const SceneNode& node)
{
	/* lambda expressions
	creates anonymous function object
	1. [] = capture list
	2. parameter list
	3. return type has the syntax -> type
	4. function body enclosedin {}
	*/
	// find node in children, using std::find_if, returns an iterator
	auto found = std::find_if(nChildren.begin(), nChildren.end(),
		[&](Ptr& p) -> bool { return p.get() == &node; });		// capture memory address and compare with lambda function

	// assertion, check validity
	assert(found != nChildren.end());


	// move node out of container
	Ptr result = std::move(*found);			// access iterator value
	result->nParent = nullptr;
	nChildren.erase(found);				// erase from vector, with iterator as parameter
	return result;
}


// drawing and rendering
void SceneNode::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	// use overloaded * operator to combine parent's absolute transform to current node's relative transform
	// get from states
	states.transform *= getTransform();		// this contain absolute world's transform

	drawCurrent(target, states);		// draw on current node
	drawChildren(target, states);
}

void SceneNode::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	// do nothing by default, to be overidden
}

void SceneNode::drawChildren(sf::RenderTarget& target, sf::RenderStates states) const
{
	// draw on all children nodes
	for (const Ptr& child : nChildren)			// range-based for loop
	{
		child->draw(target, states);			// draw on each child
	}
}



/* updating the scene */
void SceneNode::update(sf::Time dt, CommandQueue& commands)
{
	updateCurrent(dt, commands);
	updateChildren(dt, commands);
}

void SceneNode::updateCurrent(sf::Time, CommandQueue&)
{
	// to be overidden
}

void SceneNode::updateChildren(sf::Time dt, CommandQueue& commands)
{
	for (const Ptr& child : nChildren)
	{
		child->update(dt, commands);
	}
}


// for absolute transforms
/*
step up the class hierarchy and accumulate all relative transforms until we reach the root
*/
sf::Transform SceneNode::getWorldTransform() const
{
	// identity transform below, does not have any affect on the object
	sf::Transform transform = sf::Transform::Identity;		
	for (const SceneNode* node = this; node != nullptr; node = node->nParent)		// traverse all parent nodes
	{
		// use transformable getTransform method and use * operator to combine the transform
		transform = node->getTransform() * transform;
	}

	return transform;
}

sf::Vector2f SceneNode::getWorldPosition() const
{
	// transform to vector2f
	return getWorldTransform() * sf::Vector2f();
}


// get category for commands
unsigned int SceneNode::getCategory() const
{
	return Category::SceneVoidLayer;			// return scene for SceneNode class 
}

// execute command
// use bitwise & operator to the check that the command's receiver is the same category
void SceneNode::onCommand(const Command& command, sf::Time dt)
{
	// use bitwise & to check category
	if (command.category & getCategory())
		command.action(*this, dt);			// pass in SceneNode pointer to command

	// traverse children and execute their commands
	for (const Ptr& child : nChildren)
	{
		child->onCommand(command, dt);
	}
}

/* collisions */
// check collision between *this and its children with argument node
void SceneNode::checkNodeCollision(SceneNode& node, std::set<Pair>& collisionPairs)
{
//	std::cout << "checked\n";

	/*if (node.getCategory() == Category::PlayerCharacter)
	{
		std::cout << "character checked\n";
	}*/

	// compare to *this / parent
	// if nodes are different and if both are not yet distroyed, and if both collide
	if (this != &node && collision(*this, node)
		&& !isDestroyed() && !node.isDestroyed())
	{
		collisionPairs.insert(std::minmax(this, &node));			// minmax, returns pair with min as first
	}

	// compare to parameter node children
	for (auto& child : nChildren)
	{
		child->checkNodeCollision(node, collisionPairs);
	}
}

// check collisions between whole scene graph against all nodes
void SceneNode::checkSceneCollision(SceneNode& sceneGraph, std::set<Pair>& collisionPairs)
{
	checkNodeCollision(sceneGraph, collisionPairs);

	for (auto& child : sceneGraph.nChildren)
	{
		checkSceneCollision(*child, collisionPairs);
	}
}

/* cleaning up */
void SceneNode::removeWrecks()
{
	/* std::remove_if
	- std::removeif(first iterator, last iteraotr, predicate)
	- predicate accepts container type and returns bool
	- rearranges container so elements to NOt be removed are at the front
	*/
	/* std::mem_fn
	-> create a function that return true if and only if the member function passed is true, in this case MarkedforRoemval
	*/
	auto removeBegin = std::remove_if(nChildren.begin(), nChildren.end(), std::mem_fn(&SceneNode::isMarkedForRemoval));

	// removebegin returns starting iterator to where elements are to be removed
	nChildren.erase(removeBegin, nChildren.end());

	// recursively call to remove wrecks from children
	std::for_each(nChildren.begin(), nChildren.end(), std::mem_fn(&SceneNode::removeWrecks));
}


// for base return empty
sf::FloatRect SceneNode::getBoundingRect() const
{
	return sf::FloatRect();
}



bool SceneNode::isMarkedForRemoval() const
{
	// remove by default if entitiy is destroyed
	return isDestroyed();
}

bool SceneNode::isDestroyed() const
{
	// scenes do not need to be removed
	return false;
}



// get distance between two scenenodes
float distance(const SceneNode& left, const SceneNode& right)
{
	return length(left.getWorldPosition() - right.getWorldPosition());
}

// check if collide
bool collision(const SceneNode& left, const SceneNode& right)
{
	return left.getBoundingRect().intersects(right.getBoundingRect());
}
