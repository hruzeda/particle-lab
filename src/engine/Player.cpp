#include "../../inc/engine/Player.h"

#include <SFML/Network/Packet.hpp>
#include <algorithm>
#include <map>
#include <string>

#include "../../inc/engine/Aircraft.h"
#include "../../inc/engine/CommandQueue.h"
#include "../../inc/engine/Foreach.h"

using namespace std::placeholders;

struct AircraftMover {
  AircraftMover(float vx, float vy, int identifier)
      : velocity(vx, vy), aircraftID(identifier) {}

  void operator()(Aircraft& aircraft, sf::Time) const {
    if (aircraft.getIdentifier() == aircraftID)
      aircraft.accelerate(velocity * aircraft.getMaxSpeed());
  }

  sf::Vector2f velocity;
  int aircraftID;
};

struct AircraftFireTrigger {
  AircraftFireTrigger(int identifier) : aircraftID(identifier) {}

  void operator()(Aircraft& aircraft, sf::Time) const {
    if (aircraft.getIdentifier() == aircraftID) aircraft.fire();
  }

  int aircraftID;
};

struct AircraftMissileTrigger {
  AircraftMissileTrigger(int identifier) : aircraftID(identifier) {}

  void operator()(Aircraft& aircraft, sf::Time) const {
    if (aircraft.getIdentifier() == aircraftID) aircraft.launchMissile();
  }

  int aircraftID;
};

Player::Player(sf::TcpSocket* socket, sf::Int32 identifier,
               const KeyBinding* binding) {
  mKeyBinding = binding;
  mCurrentMissionStatus = MissionRunning;
  mIdentifier = identifier;
  mSocket = socket;

  // Set initial action bindings
  initializeActions();

  // Assign all categories to player's aircraft
  FOREACH(auto& pair, mActionBinding)
  pair.second.category = Category::PlayerAircraft;
}

void Player::handleEvent(const sf::Event& event, CommandQueue& commands) {
  // Event
  if (event.type == sf::Event::KeyPressed) {
    Action action;
    if (mKeyBinding && mKeyBinding->checkAction(event.key.code, action) &&
        !isRealtimeAction(action)) {
      commands.push(mActionBinding[action]);
    }
  }
}

void Player::handleRealtimeInput(CommandQueue& commands) {
  // Lookup all actions and push corresponding commands to queue
  std::vector<Action> activeActions = mKeyBinding->getRealtimeActions();
  FOREACH(Action action, activeActions)
  commands.push(mActionBinding[action]);
}

void Player::setMissionStatus(MissionStatus status) {
  mCurrentMissionStatus = status;
}

Player::MissionStatus Player::getMissionStatus() const {
  return mCurrentMissionStatus;
}

void Player::initializeActions() {
  mActionBinding[PlayerAction::MoveLeft].action =
      derivedAction<Aircraft>(AircraftMover(-1, 0, mIdentifier));
  mActionBinding[PlayerAction::MoveRight].action =
      derivedAction<Aircraft>(AircraftMover(+1, 0, mIdentifier));
  mActionBinding[PlayerAction::MoveUp].action =
      derivedAction<Aircraft>(AircraftMover(0, -1, mIdentifier));
  mActionBinding[PlayerAction::MoveDown].action =
      derivedAction<Aircraft>(AircraftMover(0, +1, mIdentifier));
  mActionBinding[PlayerAction::Fire].action =
      derivedAction<Aircraft>(AircraftFireTrigger(mIdentifier));
  mActionBinding[PlayerAction::LaunchMissile].action =
      derivedAction<Aircraft>(AircraftMissileTrigger(mIdentifier));
}
