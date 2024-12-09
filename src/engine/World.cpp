#include "../../inc/engine/World.h"

#include <SFML/Graphics/RenderTarget.hpp>
#include <algorithm>
#include <cmath>
#include <limits>

#include "../../inc/engine/Foreach.h"
#include "../../inc/engine/ParticleNode.h"
#include "../../inc/engine/Pickup.h"
#include "../../inc/engine/Projectile.h"
#include "../../inc/engine/SoundNode.h"
#include "../../inc/engine/TextNode.h"
#include "../../inc/engine/Utility.h"

World::World(sf::RenderTarget& outputTarget, FontHolder& fonts,
             SoundPlayer& sounds)
    : mTarget(outputTarget),
      mSceneTexture(),
      mWorldView(outputTarget.getDefaultView()),
      mTextures(),
      mFonts(fonts),
      mSounds(sounds),
      mSceneGraph(),
      mSceneLayers(),
      mWorldBounds(0.f, 0.f, mWorldView.getSize().x, mWorldView.getSize().y),
      mSpawnPosition(mWorldView.getSize().x / 2.f,
                     mWorldView.getSize().y / 2.f),
      mParticles(),
      mFinishSprite(nullptr) {
  mSceneTexture.create(mTarget.getSize().x, mTarget.getSize().y);

  loadTextures();
  buildScene();

  // Prepare the view
  mWorldView.setCenter(mSpawnPosition);
}

void World::update(sf::Time dt) {
  FOREACH(Quark * a, mParticles)
  a->setVelocity(0.f, 0.f);

  // Setup commands to destroy entities, and guide missiles
  // destroyEntitiesOutsideView();

  // Forward commands to scene graph, adapt velocity (scrolling, diagonal
  // correction)
  while (!mCommandQueue.isEmpty())
    mSceneGraph.onCommand(mCommandQueue.pop(), dt);

  // Collision detection and response (may destroy entities)
  handleCollisions();

  // Remove aircrafts that were destroyed (World::removeWrecks() only destroys
  // the entities, not the pointers in mPlayerAircraft)
  auto firstToRemove = std::remove_if(mParticles.begin(), mParticles.end(),
                                      std::mem_fn(&Quark::isMarkedForRemoval));
  mParticles.erase(firstToRemove, mParticles.end());

  // Remove all destroyed entities, create new ones
  mSceneGraph.removeWrecks();

  // Regular update step, adapt position (correct if outside view)
  mSceneGraph.update(dt, mCommandQueue);

  updateSounds();
}

void World::draw() {
  if (PostEffect::isSupported()) {
    mSceneTexture.clear();
    mSceneTexture.setView(mWorldView);
    mSceneTexture.draw(mSceneGraph);
    mSceneTexture.display();
    mBloomEffect.apply(mSceneTexture, mTarget);
  } else {
    mTarget.setView(mWorldView);
    mTarget.draw(mSceneGraph);
  }
}

CommandQueue& World::getCommandQueue() { return mCommandQueue; }

Quark* World::getParticle(int identifier) const {
  FOREACH(Quark * a, mParticles) {
    if (a->getIdentifier() == identifier) return a;
  }

  return nullptr;
}

void World::removeParticle(int identifier) {
  Quark* particle = getParticle(identifier);
  if (particle) {
    // particle->destroy();
    mParticles.erase(std::find(mParticles.begin(), mParticles.end(), particle));
  }
}

Quark* World::addParticle(int identifier, Quark::Type type) {
  std::unique_ptr<Quark> player(new Quark(identifier, type, mFonts));
  player->setPosition(mWorldView.getCenter());

  mParticles.push_back(player.get());
  mSceneLayers[UpperAir]->attachChild(std::move(player));
  return mParticles.back();
}

void World::setCurrentBattleFieldPosition(float lineY) {
  mWorldView.setCenter(mWorldView.getCenter().x,
                       lineY - mWorldView.getSize().y / 2);
  mSpawnPosition.y = mWorldBounds.height;
}

void World::setWorldHeight(float height) { mWorldBounds.height = height; }

void World::loadTextures() {
  mTextures.load(Textures::Entities, "media/Textures/Entities.png");
  mTextures.load(Textures::Explosion, "media/Textures/Explosion.png");
  mTextures.load(Textures::Particle, "media/Textures/Particle.png");
  mTextures.load(Textures::FinishLine, "media/Textures/FinishLine.png");
}

bool matchesCategories(SceneNode::Pair& colliders, Category::Type type1,
                       Category::Type type2) {
  unsigned int category1 = colliders.first->getCategory();
  unsigned int category2 = colliders.second->getCategory();

  // Make sure first pair entry has category type1 and second has type2
  if (type1 & category1 && type2 & category2) {
    return true;
  } else if (type1 & category2 && type2 & category1) {
    std::swap(colliders.first, colliders.second);
    return true;
  } else {
    return false;
  }
}

void World::handleCollisions() {
  std::set<SceneNode::Pair> collisionPairs;
  mSceneGraph.checkSceneCollision(mSceneGraph, collisionPairs);

  FOREACH(SceneNode::Pair pair, collisionPairs) {
    if (matchesCategories(pair, Category::PlayerAircraft,
                          Category::EnemyAircraft)) {
      auto& player = static_cast<Aircraft&>(*pair.first);
      auto& enemy = static_cast<Aircraft&>(*pair.second);

      // Collision: Player damage = enemy's remaining HP
      player.damage(enemy.getHitpoints());
      enemy.destroy();
    }

    else if (matchesCategories(pair, Category::PlayerAircraft,
                               Category::Pickup)) {
      auto& player = static_cast<Aircraft&>(*pair.first);
      auto& pickup = static_cast<Pickup&>(*pair.second);

      // Apply pickup effect to player, destroy projectile
      pickup.apply(player);
      pickup.destroy();
      player.playLocalSound(mCommandQueue, SoundEffect::CollectPickup);
    }

    else if (matchesCategories(pair, Category::EnemyAircraft,
                               Category::AlliedProjectile) ||
             matchesCategories(pair, Category::PlayerAircraft,
                               Category::EnemyProjectile)) {
      auto& aircraft = static_cast<Aircraft&>(*pair.first);
      auto& projectile = static_cast<Projectile&>(*pair.second);

      // Apply projectile damage to aircraft, destroy projectile
      aircraft.damage(projectile.getDamage());
      projectile.destroy();
    }
  }
}

void World::updateSounds() {
  sf::Vector2f listenerPosition;

  // 0 players (multiplayer mode, until server is connected) -> view center
  if (mParticles.empty()) {
    listenerPosition = mWorldView.getCenter();
  }

  // 1 or more players -> mean position between all aircrafts
  else {
    FOREACH(Quark * particle, mParticles)
    listenerPosition += particle->getWorldPosition();

    listenerPosition /= static_cast<float>(mParticles.size());
  }

  // Set listener's position
  mSounds.setListenerPosition(listenerPosition);

  // Remove unused sounds
  mSounds.removeStoppedSounds();
}

void World::buildScene() {
  // Initialize the different layers
  for (std::size_t i = 0; i < LayerCount; ++i) {
    Category::Type category =
        (i == LowerAir) ? Category::SceneAirLayer : Category::None;

    SceneNode::Ptr layer(new SceneNode(category));
    mSceneLayers[i] = layer.get();

    mSceneGraph.attachChild(std::move(layer));
  }

  // Add the finish line to the scene
  sf::Texture& finishTexture = mTextures.get(Textures::FinishLine);
  std::unique_ptr<SpriteNode> finishSprite(new SpriteNode(finishTexture));
  finishSprite->setPosition(0.f, -76.f);
  mFinishSprite = finishSprite.get();
  mSceneLayers[Background]->attachChild(std::move(finishSprite));

  // Add particle node to the scene
  std::unique_ptr<ParticleNode> smokeNode(
      new ParticleNode(Particle::Smoke, mTextures));
  mSceneLayers[LowerAir]->attachChild(std::move(smokeNode));

  // Add propellant particle node to the scene
  std::unique_ptr<ParticleNode> propellantNode(
      new ParticleNode(Particle::Propellant, mTextures));
  mSceneLayers[LowerAir]->attachChild(std::move(propellantNode));

  // Add sound effect node
  std::unique_ptr<SoundNode> soundNode(new SoundNode(mSounds));
  mSceneGraph.attachChild(std::move(soundNode));
}

void World::destroyEntitiesOutsideView() {
  Command command;
  command.category = Category::Projectile | Category::EnemyAircraft;
  command.action = derivedAction<Entity>([this](Entity& e, sf::Time) {
    if (!getBattlefieldBounds().intersects(e.getBoundingRect())) e.remove();
  });

  mCommandQueue.push(command);
}

sf::FloatRect World::getViewBounds() const {
  return sf::FloatRect(mWorldView.getCenter() - mWorldView.getSize() / 2.f,
                       mWorldView.getSize());
}

sf::FloatRect World::getBattlefieldBounds() const {
  // Return view bounds + some area at top, where enemies spawn
  sf::FloatRect bounds = getViewBounds();
  bounds.top -= 100.f;
  bounds.height += 100.f;

  return bounds;
}
