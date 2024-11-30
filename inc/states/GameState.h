#ifndef GAMESTATE_HEADER
#define GAMESTATE_HEADER

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

#include "../engine/Player.h"
#include "../engine/World.h"
#include "State.h"

class GameState : public State {
 public:
  GameState(StateStack& stack, Context context);

  virtual void draw();
  virtual bool update(sf::Time dt);
  virtual bool handleEvent(const sf::Event& event);

 private:
  World mWorld;
  Player mPlayer;
};

#endif  // GAMESTATE_HEADER