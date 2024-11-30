#ifndef GAMEOVERSTATE_HEADER
#define GAMEOVERSTATE_HEADER

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

#include "../engine/Container.h"
#include "State.h"

class GameOverState : public State {
 public:
  GameOverState(StateStack& stack, Context context, const std::string& text);

  virtual void draw();
  virtual bool update(sf::Time dt);
  virtual bool handleEvent(const sf::Event& event);

 private:
  sf::Text mGameOverText;
  sf::Time mElapsedTime;
};

#endif  // GAMEOVERSTATE_HEADER
