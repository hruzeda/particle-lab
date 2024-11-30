#ifndef COMMANDQUEUE_HEADER
#define COMMANDQUEUE_HEADER

#include <queue>

#include "Command.h"

class CommandQueue {
 public:
  void push(const Command& command);
  Command pop();
  bool isEmpty() const;

 private:
  std::queue<Command> mQueue;
};

#endif  // COMMANDQUEUE_HEADER
