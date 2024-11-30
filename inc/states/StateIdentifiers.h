#ifndef STATEIDENTIFIERS_HEADER
#define STATEIDENTIFIERS_HEADER

namespace States {
enum ID {
  None,
  Title,
  Menu,
  Game,
  Loading,
  Pause,
  NetworkPause,
  Settings,
  GameOver,
  MissionSuccess,
  HostGame,
  JoinGame,
};
}

#endif  // STATEIDENTIFIERS_HEADER
