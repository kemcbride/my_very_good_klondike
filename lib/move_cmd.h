#ifndef MOVE_CMD_H
#define MOVE_CMD_H

#include <iostream>
#include <stdexcept>
#include <string>

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

  static Source parseSource(std::string);
  int parseCount(std::string);
  static Dest parseDest(std::string);

  static bool validateFdn(std::string);
  static bool validatePile(std::string);

 public:
  MoveCmd(std::string);
  MoveCmd(char, int, char, int, int);
  MoveCmd(Source, Dest, int);

  static MoveCmd create();

  Source getSource() const;
  Dest getDest() const;
  int getCount() const;

  const std::string toString() const;
};

#endif  // MOVE_CMD_H
