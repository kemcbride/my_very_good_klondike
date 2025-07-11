#ifndef GAMELOG_CMD_H
#define GAMELOG_CMD_H

#include <string>

#include "command.h"
#include "move.h"

class GameLogCmd : public Command {
 private:
  GameLogCmd() = delete;
  std::string cmd;

 public:
  GameLogCmd(std::string);
  const std::string toString() const;
};

#endif  // GAMELOG_CMD_H
