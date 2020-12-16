#ifndef SCENENODE_HPP
#define SCENENODE_HPP

// node class for tree graph, for managing transforms

#include <memory>
#include <vector>

class SceneNode
{
public:
	typedef std::unique_ptr<SceneNode> Ptr;			// rename unique_ptr to Ptr

public:
	SceneNode();

private:
	std::vector<Ptr> nChildren;				// use unique_ptr<SceneNode> not SceneNode as std::vector has to be a completed type
	SceneNode* nParent;						// pointer to parent node
};


#endif