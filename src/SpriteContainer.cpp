#include "SpriteContainer.hpp"
#include "Command.hpp"
#include "utils.hpp"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include <functional>
#include <algorithm>
#include <cassert>
#include <cmath>

SpriteContainer::SpriteContainer(Category::Type category, PhysicsWorld &world, Entity entity)
	: mChildren(), mParent(nullptr), mDefaultCategory(category), RigidBody(world, entity)
{
}

void SpriteContainer::attachChild(Ptr child)
{
	child->mParent = this;
	mChildren.push_back(std::move(child));
}

SpriteContainer::Ptr SpriteContainer::detachChild(const SpriteContainer &node)
{
	auto found = std::find_if(mChildren.begin(), mChildren.end(), [&](Ptr &p)
							  { return p.get() == &node; });
	assert(found != mChildren.end());

	Ptr result = std::move(*found);
	result->mParent = nullptr;
	mChildren.erase(found);
	return result;
}

void SpriteContainer::update(sf::Time dt, CommandQueue &commands)
{
	updateCurrent(dt, commands);
	updateChildren(dt, commands);
}

void SpriteContainer::updateCurrent(sf::Time, CommandQueue &)
{
	// Do nothing by default
}

void SpriteContainer::updateChildren(sf::Time dt, CommandQueue &commands)
{
	for (auto &child : mChildren)
		child->update(dt, commands);
}

void SpriteContainer::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	// Apply transform of current node
	states.transform *= Transformable::getTransform();

	// Draw node and children with changed transform
	drawCurrent(target, states);
	drawChildren(target, states);

	// Draw bounding rectangle - disabled by default
	// drawBoundingRect(target, states);
}

void SpriteContainer::drawCurrent(sf::RenderTarget &, sf::RenderStates) const
{
	// Do nothing by default
}

void SpriteContainer::drawChildren(sf::RenderTarget &target, sf::RenderStates states) const
{
	for (auto &child : mChildren)
		child->draw(target, states);
}

void SpriteContainer::drawBoundingRect(sf::RenderTarget &target, sf::RenderStates) const
{
	sf::FloatRect rect = getBoundingRect();

	sf::RectangleShape shape;
	shape.setPosition(sf::Vector2f(rect.left, rect.top));
	shape.setSize(sf::Vector2f(rect.width, rect.height));
	shape.setFillColor(sf::Color::Transparent);
	shape.setOutlineColor(sf::Color::Green);
	shape.setOutlineThickness(1.f);

	target.draw(shape);
}

sf::Vector2f SpriteContainer::getWorldPosition() const
{
	return getWorldTransform() * sf::Vector2f();
}

sf::Transform SpriteContainer::getWorldTransform() const
{
	sf::Transform transform = sf::Transform::Identity;

	for (const SpriteContainer *node = this; node != nullptr; node = node->mParent)
		transform = node->sf::Transformable::getTransform() * transform;

	return transform;
}

void SpriteContainer::onCommand(const Command &command, sf::Time dt)
{
	// Command current node, if category matches
	if (command.category & getCategory())
		command.action(*this, dt);

	// Command children
	for (auto &child : mChildren)
		child->onCommand(command, dt);
}

unsigned int SpriteContainer::getCategory() const
{
	return mDefaultCategory;
}

void SpriteContainer::checkSceneCollision(SpriteContainer &sceneGraph, std::set<Pair> &collisionPairs)
{
	checkNodeCollision(sceneGraph, collisionPairs);

	for (auto &child : sceneGraph.mChildren)
		checkSceneCollision(*child, collisionPairs);
}

void SpriteContainer::checkNodeCollision(SpriteContainer &node, std::set<Pair> &collisionPairs)
{
	if (this != &node && collision(*this, node) && !isDestroyed() && !node.isDestroyed())
		collisionPairs.insert(std::minmax(this, &node));

	for (auto &child : mChildren)
		child->checkNodeCollision(node, collisionPairs);
}

void SpriteContainer::removeWrecks()
{
	// Remove all children which request so
	auto wreckfieldBegin = std::remove_if(mChildren.begin(), mChildren.end(), std::mem_fn(&SpriteContainer::isMarkedForRemoval));
	mChildren.erase(wreckfieldBegin, mChildren.end());

	// Call function recursively for all remaining children
	std::for_each(mChildren.begin(), mChildren.end(), std::mem_fn(&SpriteContainer::removeWrecks));
}

sf::FloatRect SpriteContainer::getBoundingRect() const
{
	return sf::FloatRect();
}

bool SpriteContainer::isMarkedForRemoval() const
{
	// By default, remove node if entity is destroyed
	return isDestroyed();
}

bool SpriteContainer::isDestroyed() const
{
	// By default, scene node needn't be removed
	return false;
}

bool collision(const SpriteContainer &lhs, const SpriteContainer &rhs)
{
	return lhs.getBoundingRect().intersects(rhs.getBoundingRect());
}

float distance(const SpriteContainer &lhs, const SpriteContainer &rhs)
{
	return length(lhs.getWorldPosition() - rhs.getWorldPosition());
}
