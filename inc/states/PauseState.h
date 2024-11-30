#ifndef PAUSESTATE_HEADER
#define PAUSESTATE_HEADER

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

#include "../engine/Container.h"
#include "State.h"

class PauseState : public State {
 public:
  PauseState(StateStack& stack, Context context,
             bool letUpdatesThrough = false);
  ~PauseState();

  virtual void draw();
  virtual bool update(sf::Time dt);
  virtual bool handleEvent(const sf::Event& event);

 private:
  sf::Sprite mBackgroundSprite;
  sf::Text mPausedText;
  GUI::Container mGUIContainer;
  bool mLetUpdatesThrough;
};

#endif  // PAUSESTATE_HEADER