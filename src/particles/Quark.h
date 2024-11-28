#pragma once

#include "../SpriteContainer.h"
#include <SFML/Graphics.hpp>

class Quark : public SpriteContainer
{
public:
    Quark(sf::Color color, sf::Vector2f position, const std::string &text, PhysicsWorld &world, Entity entity);

    // Delete copy constructor and copy assignment operator
    Quark(const Quark &) = delete;
    Quark &operator=(const Quark &) = delete;

    // Add getLabel method
    const sf::Text &getLabel() const { return label; }

protected:
    sf::Text label;
    sf::Font font;
    sf::CircleShape shape;
};

class UpQuark : public Quark
{
public:
    UpQuark(sf::Color color, sf::Vector2f position, PhysicsWorld &world, Entity entity);
};

class DownQuark : public Quark
{
public:
    DownQuark(sf::Color color, sf::Vector2f position, PhysicsWorld &world, Entity entity);
};