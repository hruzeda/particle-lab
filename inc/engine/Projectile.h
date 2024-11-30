#ifndef PROJECTILE_HEADER
#define PROJECTILE_HEADER

#include <SFML/Graphics/Sprite.hpp>

#include "Entity.h"
#include "ResourceIdentifiers.h"

class Projectile : public Entity {
 public:
  enum Type { AlliedBullet, EnemyBullet, Missile, TypeCount };

 public:
  Projectile(Type type, const TextureHolder& textures);

  void guideTowards(sf::Vector2f position);
  bool isGuided() const;

  virtual unsigned int getCategory() const;
  virtual sf::FloatRect getBoundingRect() const;
  float getMaxSpeed() const;
  int getDamage() const;

 private:
  virtual void updateCurrent(sf::Time dt, CommandQueue& commands);
  virtual void drawCurrent(sf::RenderTarget& target,
                           sf::RenderStates states) const;

 private:
  Type mType;
  sf::Sprite mSprite;
  sf::Vector2f mTargetDirection;
};

#endif  // PROJECTILE_HEADER