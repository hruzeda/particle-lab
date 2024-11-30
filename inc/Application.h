#ifndef APPLICATION_HEADER
#define APPLICATION_HEADER

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Time.hpp>

#include "engine/KeyBinding.h"
#include "engine/MusicPlayer.h"
#include "engine/ResourceHolder.h"
#include "engine/ResourceIdentifiers.h"
#include "engine/SoundPlayer.h"
#include "states/StateStack.h"

class Application {
 public:
  Application();
  void run();

 private:
  void processInput();
  void update(sf::Time dt);
  void render();

  void updateStatistics(sf::Time dt);
  void registerStates();

 private:
  static const sf::Time TimePerFrame;

  sf::RenderWindow mWindow;
  TextureHolder mTextures;
  FontHolder mFonts;
  MusicPlayer mMusic;
  SoundPlayer mSounds;

  KeyBinding mKeyBinding1;
  KeyBinding mKeyBinding2;
  StateStack mStateStack;

  sf::Text mStatisticsText;
  sf::Time mStatisticsUpdateTime;
  std::size_t mStatisticsNumFrames;
};

#endif  // APPLICATION_HEADER
