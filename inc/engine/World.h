#ifndef WORLD_HEADER
#define WORLD_HEADER

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/System/NonCopyable.hpp>
#include <array>
#include <queue>

#include "Aircraft.h"
#include "BloomEffect.h"
#include "Command.h"
#include "CommandQueue.h"
#include "NetworkProtocol.h"
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
  World(sf::RenderTarget& outputTarget, FontHolder& fonts, SoundPlayer& sounds,
        bool networked = false);
  void update(sf::Time dt);
  void draw();

  sf::FloatRect getViewBounds() const;
  CommandQueue& getCommandQueue();
  Aircraft* addAircraft(int identifier);
  void removeAircraft(int identifier);
  void setCurrentBattleFieldPosition(float lineY);
  void setWorldHeight(float height);

  void addEnemy(Aircraft::Type type, float relX, float relY);
  void sortEnemies();

  bool hasAlivePlayer() const;
  bool hasPlayerReachedEnd() const;

  void setWorldScrollCompensation(float compensation);

  Aircraft* getAircraft(int identifier) const;
  sf::FloatRect getBattlefieldBounds() const;

  void createPickup(sf::Vector2f position, Pickup::Type type);
  bool pollGameAction(GameActions::Action& out);

 private:
  void loadTextures();
  void adaptPlayerPosition();
  void adaptPlayerVelocity();
  void handleCollisions();
  void updateSounds();

  void buildScene();
  void addEnemies();
  void spawnEnemies();
  void destroyEntitiesOutsideView();
  void guideMissiles();

 private:
  enum Layer { Background, LowerAir, UpperAir, LayerCount };

  struct SpawnPoint {
    SpawnPoint(Aircraft::Type type, float x, float y)
        : type(type), x(x), y(y) {}

    Aircraft::Type type;
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
  float mScrollSpeed;
  float mScrollSpeedCompensation;
  std::vector<Aircraft*> mPlayerAircrafts;

  std::vector<SpawnPoint> mEnemySpawnPoints;
  std::vector<Aircraft*> mActiveEnemies;

  BloomEffect mBloomEffect;

  bool mNetworkedWorld;
  NetworkNode* mNetworkNode;
  SpriteNode* mFinishSprite;
};

#endif  // WORLD_HEADER
