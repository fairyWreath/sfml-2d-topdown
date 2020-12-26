#include "SoundPlayer.hpp"
#include "SoundNode.hpp"

SoundNode::SoundNode(SoundPlayer& player) :
	SceneNode(),
	nSounds(player)
{

}

void SoundNode::playSound(SoundEffect::ID sound, sf::Vector2f position)
{
	nSounds.play(sound, position);
}

unsigned int SoundNode::getCategory() const
{
	return Category::SoundEffect;
}