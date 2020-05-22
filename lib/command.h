#ifndef CMD_H
#define CMD_H

#include <string>

class Command {
private:
  std::string cmd;

public:
  Command() = default;
  Command(std::string);

  std::string virtual toString();
};

#endif // CMD_H
