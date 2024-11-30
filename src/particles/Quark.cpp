#include "../../inc/particles/Quark.h"

// using namespace reactphysics3d;

Quark::Quark(sf::Color color, const std::string &text) {
  shape.setRadius(10.f);
  shape.setPosition(sf::Vector2f(0.f, 0.f));
  shape.setFillColor(color);

  if (!font.loadFromFile("PermanentMarker-Regular.ttf")) {
    // Handle error
  }
  label.setFont(font);
  label.setString(text);
  label.setCharacterSize(15);
  label.setFillColor(sf::Color::White);
  label.setPosition(5.5f, -1.25f);
}

void Quark::draw(sf::RenderTarget &target, sf::RenderStates states) const {
  target.draw(shape, states);
  target.draw(label, states);
}

UpQuark::UpQuark(sf::Color color) : Quark(color, "U") {}

DownQuark::DownQuark(sf::Color color) : Quark(color, "D") {}