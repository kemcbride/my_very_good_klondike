#ifndef CMD_H
#define CMD_H

#include <string>

class Command {
private:
  std::string cmd;

public:
  Command() = default;
  Command(std::string);
  virtual ~Command() = 0;

  const std::string virtual toString() const;
};

#endif // CMD_H
