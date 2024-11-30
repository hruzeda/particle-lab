#ifndef SOUNDNODE_HEADER
#define SOUNDNODE_HEADER

#include "ResourceIdentifiers.h"
#include "SceneNode.h"

class SoundPlayer;

class SoundNode : public SceneNode {
 public:
  explicit SoundNode(SoundPlayer& player);
  void playSound(SoundEffect::ID sound, sf::Vector2f position);

  virtual unsigned int getCategory() const;

 private:
  SoundPlayer& mSounds;
};

#endif  // SOUNDNODE_HEADER
