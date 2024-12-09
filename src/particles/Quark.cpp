#include "../../inc/particles/Quark.h"

#include <cmath>

// using namespace reactphysics3d;

Quark::Quark(int identifier, Quark::Type type, const FontHolder& fonts)
    : Entity(1),
      identifier(identifier),
      type(type),
      color(sf::Color::Red),
      shape(10),
      font() {
  shape.setRadius(10.f);
  sf::FloatRect bounds = shape.getLocalBounds();
  shape.setOrigin(std::floor(bounds.left + bounds.width / 2.f),
                  std::floor(bounds.top + bounds.height / 2.f));
  shape.setFillColor(color);

  std::unique_ptr<TextNode> label(new TextNode(fonts, "U"));
  mLabel = label.get();
  mLabel->setPosition(5.5f, -1.25f);
  attachChild(std::move(label));
}

void Quark::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  target.draw(shape, states);
}
