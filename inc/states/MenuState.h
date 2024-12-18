#ifndef MENUSTATE_HEADER
#define MENUSTATE_HEADER

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

#include "../engine/Container.h"
#include "State.h"

class MenuState : public State {
 public:
  MenuState(StateStack& stack, Context context);

  virtual void draw();
  virtual bool update(sf::Time dt);
  virtual bool handleEvent(const sf::Event& event);

 private:
  sf::Sprite mBackgroundSprite;
  GUI::Container mGUIContainer;
};

#endif  // MENUSTATE_HEADER
