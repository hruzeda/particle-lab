#include "../../inc/states/TitleState.h"

#include <SFML/Graphics/RenderWindow.hpp>

#include "../../inc/engine/ResourceHolder.h"
#include "../../inc/engine/Utility.h"

TitleState::TitleState(StateStack& stack, Context context)
    : State(stack, context),
      mText(),
      mShowText(true),
      mTextEffectTime(sf::Time::Zero) {
  mText.setFont(context.fonts->get(Fonts::Main));
  mText.setString("Press any key to start");
  centerOrigin(mText);
  mText.setPosition(sf::Vector2f(context.window->getSize() / 2u));
}

void TitleState::draw() {
  sf::RenderWindow& window = *getContext().window;

  if (mShowText) window.draw(mText);
}

bool TitleState::update(sf::Time dt) {
  mTextEffectTime += dt;

  if (mTextEffectTime >= sf::seconds(0.5f)) {
    mShowText = !mShowText;
    mTextEffectTime = sf::Time::Zero;
  }

  return true;
}

bool TitleState::handleEvent(const sf::Event& event) {
  // If any key is pressed, trigger the next screen
  if (event.type == sf::Event::KeyReleased) {
    requestStackPop();
    requestStackPush(States::Menu);
  }

  return true;
}