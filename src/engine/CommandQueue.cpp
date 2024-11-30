#include "../../inc/engine/CommandQueue.h"

#include "../../inc/engine/SceneNode.h"

void CommandQueue::push(const Command& command) { mQueue.push(command); }

Command CommandQueue::pop() {
  Command command = mQueue.front();
  mQueue.pop();
  return command;
}

bool CommandQueue::isEmpty() const { return mQueue.empty(); }
