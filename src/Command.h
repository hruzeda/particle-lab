#ifndef COMMAND_H
#define COMMAND_H

#include "Category.h"
#include "SpriteContainer.h"

#include <SFML/System/Time.hpp>

#include <functional>
#include <cassert>

struct Command
{
    typedef std::function<void(SpriteContainer &, sf::Time)> Action;

    Command();

    Action action;
    unsigned int category;
};

template <typename GameObject, typename Function>
Command::Action derivedAction(Function fn)
{
    return [=](SpriteContainer &node, sf::Time dt)
    {
        // Check if cast is safe
        assert(dynamic_cast<GameObject *>(&node) != nullptr);

        // Downcast node and invoke function on it
        fn(static_cast<GameObject &>(node), dt);
    };
}

#endif // COMMAND_H
