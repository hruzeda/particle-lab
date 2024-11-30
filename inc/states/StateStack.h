#ifndef STATESTACK_HEADER
#define STATESTACK_HEADER

#include <SFML/System/NonCopyable.hpp>
#include <SFML/System/Time.hpp>
#include <functional>
#include <map>
#include <utility>
#include <vector>

#include "../engine/ResourceIdentifiers.h"
#include "State.h"
#include "StateIdentifiers.h"

namespace sf {
class Event;
class RenderWindow;
}  // namespace sf

class StateStack : private sf::NonCopyable {
 public:
  enum Action {
    Push,
    Pop,
    Clear,
  };

 public:
  explicit StateStack(State::Context context);

  template <typename T>
  void registerState(States::ID stateID);
  template <typename T, typename Param1>
  void registerState(States::ID stateID, Param1 arg1);

  void update(sf::Time dt);
  void draw();
  void handleEvent(const sf::Event& event);

  void pushState(States::ID stateID);
  void popState();
  void clearStates();

  bool isEmpty() const;

 private:
  State::Ptr createState(States::ID stateID);
  void applyPendingChanges();

 private:
  struct PendingChange {
    explicit PendingChange(Action action, States::ID stateID = States::None);

    Action action;
    States::ID stateID;
  };

 private:
  std::vector<State::Ptr> mStack;
  std::vector<PendingChange> mPendingList;

  State::Context mContext;
  std::map<States::ID, std::function<State::Ptr()>> mFactories;
};

template <typename T>
void StateStack::registerState(States::ID stateID) {
  mFactories[stateID] = [this]() { return State::Ptr(new T(*this, mContext)); };
}

template <typename T, typename Param1>
void StateStack::registerState(States::ID stateID, Param1 arg1) {
  mFactories[stateID] = [this, arg1]() {
    return State::Ptr(new T(*this, mContext, arg1));
  };
}

#endif  // STATESTACK_HEADER
