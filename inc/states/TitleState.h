#ifndef TITLESTATE_HEADER
#define TITLESTATE_HEADER

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

#include "State.h"

class TitleState : public State {
 public:
  TitleState(StateStack& stack, Context context);

  virtual void draw();
  virtual bool update(sf::Time dt);
  virtual bool handleEvent(const sf::Event& event);

 private:
  sf::Sprite mBackgroundSprite;
  sf::Text mText;

  bool mShowText;
  sf::Time mTextEffectTime;
};

#endif  // TITLESTATE_HEADER
