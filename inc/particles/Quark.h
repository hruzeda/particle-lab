#ifndef QUARK_HEADER
#define QUARK_HEADER

#include <SFML/Graphics.hpp>

#include "../../inc/engine/SpriteNode.h"

// using namespace reactphysics3d;

class Quark : public SpriteNode {
 public:
  enum Type { Up, Down, Strange, Charm, Top, Bottom };
  Quark(int identifier, Type type);

  // Delete copy constructor and copy assignment operator
  Quark(const Quark &) = delete;
  Quark &operator=(const Quark &) = delete;

  int getIdentifier() const { return identifier; }
  Type getType() const { return type; }

  void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

 protected:
  int identifier;
  Type type;
  sf::Color color;
  sf::CircleShape shape;
  sf::Text label;
  sf::Font font;
};

#endif  // QUARK_HEADER