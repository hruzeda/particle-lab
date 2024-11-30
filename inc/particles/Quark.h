#ifndef QUARK_HEADER
#define QUARK_HEADER

#include <SFML/Graphics.hpp>

#include "../../inc/engine/SpriteNode.h"

// using namespace reactphysics3d;

class Quark : public SpriteNode {
 public:
  Quark(sf::Color color, const std::string &text);

  // Delete copy constructor and copy assignment operator
  Quark(const Quark &) = delete;
  Quark &operator=(const Quark &) = delete;

  void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

 protected:
  sf::Color _color;
  mutable sf::CircleShape shape;
  mutable sf::Text label;
  mutable sf::Font font;
};

class UpQuark : public Quark {
 public:
  UpQuark(sf::Color color);
};

class DownQuark : public Quark {
 public:
  DownQuark(sf::Color color);
};

#endif  // QUARK_HEADER