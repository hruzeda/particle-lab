#ifndef SPRITECONTAINER_H
#define SPRITECONTAINER_H

#include "Category.h"

#include <SFML/System/NonCopyable.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <reactphysics3d/reactphysics3d.h>

#include <vector>
#include <set>
#include <memory>
#include <utility>

using namespace reactphysics3d;

class CommandQueue;
class Command;

class SpriteContainer : public sf::Transformable, public sf::Drawable, public RigidBody, sf::NonCopyable
{
public:
    typedef std::unique_ptr<SpriteContainer> Ptr;
    typedef std::pair<SpriteContainer *, SpriteContainer *> Pair;

public:
    explicit SpriteContainer(PhysicsWorld &world, Entity entity, Category::Type category = Category::None);

    void attachChild(Ptr child);
    Ptr detachChild(const SpriteContainer &node);

    void update(sf::Time dt, CommandQueue &commands);

    sf::Vector2f getWorldPosition() const;
    sf::Transform getWorldTransform() const;

    void onCommand(const Command &command, sf::Time dt);
    virtual unsigned int getCategory() const;

    void checkSceneCollision(SpriteContainer &sceneGraph, std::set<Pair> &collisionPairs);
    void checkNodeCollision(SpriteContainer &node, std::set<Pair> &collisionPairs);
    void removeWrecks();
    virtual sf::FloatRect getBoundingRect() const;
    virtual bool isMarkedForRemoval() const;
    virtual bool isDestroyed() const;

private:
    virtual void updateCurrent(sf::Time dt, CommandQueue &commands);
    void updateChildren(sf::Time dt, CommandQueue &commands);

    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;
    virtual void drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const;
    void drawChildren(sf::RenderTarget &target, sf::RenderStates states) const;
    void drawBoundingRect(sf::RenderTarget &target, sf::RenderStates states) const;

private:
    std::vector<Ptr> mChildren;
    SpriteContainer *mParent;
    Category::Type mDefaultCategory;
};

bool collision(const SpriteContainer &lhs, const SpriteContainer &rhs);
float distance(const SpriteContainer &lhs, const SpriteContainer &rhs);

#endif // SPRITECONTAINER_H
