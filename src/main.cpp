#include "particles/Quark.h"

#include <reactphysics3d/reactphysics3d.h>
#include <SFML/Graphics.hpp>

using namespace reactphysics3d;

void drawProton(sf::RenderWindow &window, PhysicsWorld &world)
{
    Entity protonEntity = world.createRigidBody(); // THIS SHOULD BE COMPOSITION NOT INHERITANCE
    UpQuark upQuark1(sf::Color::Blue, sf::Vector2f(5.f, 5.f), world);
    window.draw(upQuark1);
    window.draw(upQuark1.getLabel());

    UpQuark upQuark2(sf::Color::Red, sf::Vector2f(15.f, 5.f), world);
    window.draw(upQuark2);
    window.draw(upQuark2.getLabel());

    DownQuark downQuark(sf::Color::Green, sf::Vector2f(7.5f, 15.f), world);
    window.draw(downQuark);
    window.draw(downQuark.getLabel());
}

void drawNeutron(sf::RenderWindow &window, PhysicsWorld &world)
{
    UpQuark upQuark(sf::Color::Blue, sf::Vector2f(35.f, 5.f));
    window.draw(upQuark);
    window.draw(upQuark.getLabel());

    DownQuark downQuark1(sf::Color::Red, sf::Vector2f(45.f, 5.f));
    window.draw(downQuark1);
    window.draw(downQuark1.getLabel());

    DownQuark downQuark2(sf::Color::Green, sf::Vector2f(37.5f, 15.f));
    window.draw(downQuark2);
    window.draw(downQuark2.getLabel());
}

int main()
{
    sf::RenderWindow window({640u, 480u}, "Particle Lab", sf::Style::Close, sf::ContextSettings(0, 0, 1));
    window.setFramerateLimit(60);

    PhysicsCommon physicsCommon;
    PhysicsWorld* world = physicsCommon.createPhysicsWorld();

    drawProton(window, *world);
    drawNeutron(window, *world);

    while (window.isOpen())
    {
        for (auto event = sf::Event(); window.pollEvent(event);)
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }

        window.display();
    }
}
