#include "Quark.hpp"

using namespace reactphysics3d;

Quark::Quark(sf::Color color, sf::Vector2f position, const std::string &text, PhysicsWorld &world, Entity entity) : SpriteContainer(Category::Sprite, world, entity)
{
    setPosition(position);

    shape = sf::CircleShape(10.f);
    shape.setFillColor(color);

    if (!font.loadFromFile("PermanentMarker-Regular.ttf"))
    {
        // Handle error
    }
    label = sf::Text(text, font, 20u);
    label.setFillColor(sf::Color::White);
    label.setPosition(1.f, 1.f);
}

UpQuark::UpQuark(sf::Color color, sf::Vector2f position, PhysicsWorld &world, Entity entity)
    : Quark(color, position, "u", world, entity) {}

DownQuark::DownQuark(sf::Color color, sf::Vector2f position, PhysicsWorld &world, Entity entity)
    : Quark(color, position, "d", world, entity) {}