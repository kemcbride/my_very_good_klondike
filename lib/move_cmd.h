#ifndef MOVE_CMD_H
#define MOVE_CMD_H

#include "location.h"
#include <iostream>
#include <string>

// Useful helper... why is this task annoying? Who knows.
int char_to_int(char);

class MoveCmd {
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

  Source getSource();
  Dest getDest();
  int getCount();
};

#endif // MOVE_CMD_H
