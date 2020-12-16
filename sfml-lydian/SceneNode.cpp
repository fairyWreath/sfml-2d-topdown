#include "SceneNode.hpp"

#include "Foreach.hpp"

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
void SceneNode::update(sf::Time dt)
{
	updateCurrent(dt);
	updateChildren(dt);
}

void SceneNode::updateCurrent(sf::Time)
{
	// to be overidden
}

void SceneNode::updateChildren(sf::Time dt)
{
	for (const Ptr& child : nChildren)
	{
		child->update(dt);
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