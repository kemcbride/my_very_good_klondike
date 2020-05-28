#ifndef HINT_CMD_H
#define HINT_CMD_H

#include <string>

#include "command.h"
#include "move.h"

class HintCmd : public Command {
private:
  HintCmd() = delete;
  std::string cmd;

public:
  HintCmd(std::string);
  HintCmd(Move);
  std::string toString();
};

#endif // HINT_CMD_H
