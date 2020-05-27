/* command.cpp
 * contains impl for abstract base class command
 * you play by submitting commands to the game
 * @kemcbride/ke2mcbri 2020 :o
 */


#include "command.h"

using namespace std;

Command::Command(string cmd) : cmd(cmd) {};
Command::~Command() {};

string Command::toString() {
  return this->cmd;
}
