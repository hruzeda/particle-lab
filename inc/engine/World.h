#ifndef WORLD_HEADER
#define WORLD_HEADER

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/System/NonCopyable.hpp>
#include <array>
#include <queue>

#include "../particles/Quark.h"
#include "Aircraft.h"
#include "BloomEffect.h"
#include "Command.h"
#include "CommandQueue.h"
#include "Pickup.h"
#include "ResourceHolder.h"
#include "ResourceIdentifiers.h"
#include "SceneNode.h"
#include "SoundPlayer.h"
#include "SpriteNode.h"

// Forward declaration
namespace sf {
class RenderTarget;
}

class NetworkNode;

class World : private sf::NonCopyable {
 public:
  World(sf::RenderTarget& outputTarget, FontHolder& fonts, SoundPlayer& sounds);
  void update(sf::Time dt);
  void draw();

  sf::FloatRect getViewBounds() const;
  CommandQueue& getCommandQueue();
  Quark* addParticle(int identifier, Quark::Type type);
  void removeParticle(int identifier);
  void setCurrentBattleFieldPosition(float lineY);
  void setWorldHeight(float height);

  Quark* getParticle(int identifier) const;
  sf::FloatRect getBattlefieldBounds() const;

 private:
  void loadTextures();
  void handleCollisions();
  void updateSounds();

  void buildScene();
  void destroyEntitiesOutsideView();

 private:
  enum Layer { Background, LowerAir, UpperAir, LayerCount };

  struct SpawnPoint {
    SpawnPoint(Quark::Type type, float x, float y) : type(type), x(x), y(y) {}

    Quark::Type type;
    float x;
    float y;
  };

 private:
  sf::RenderTarget& mTarget;
  sf::RenderTexture mSceneTexture;
  sf::View mWorldView;
  TextureHolder mTextures;
  FontHolder& mFonts;
  SoundPlayer& mSounds;

  SceneNode mSceneGraph;
  std::array<SceneNode*, LayerCount> mSceneLayers;
  CommandQueue mCommandQueue;

  sf::FloatRect mWorldBounds;
  sf::Vector2f mSpawnPosition;
  std::vector<Quark*> mParticles;
  BloomEffect mBloomEffect;

  bool mNetworkedWorld;
  NetworkNode* mNetworkNode;
  SpriteNode* mFinishSprite;
};

#endif  // WORLD_HEADER
