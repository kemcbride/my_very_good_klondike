#ifndef MOVE_CMD_H
#define MOVE_CMD_H

#include <string>
#include <stdexcept>

#include "command.h"
#include "location.h"

// Useful helper... why is this task annoying? Who knows.
int char_to_int(char);

class MoveCmd : public Command {
private:
  MoveCmd() = delete;
  std::string cmd;
  Source source;
  Dest dest;
  int count = 0;

  Source parseSource(std::string);
  int parseCount(std::string);
  Dest parseDest(std::string);

  bool validateFdn(std::string);
  bool validatePile(std::string);

public:
  MoveCmd(std::string);
  MoveCmd(char, int, char, int, int);
  MoveCmd(Source, Dest, int);

  Source getSource() const;
  Dest getDest() const;
  int getCount() const;

  const std::string toString() const;
};

#endif // MOVE_CMD_H
