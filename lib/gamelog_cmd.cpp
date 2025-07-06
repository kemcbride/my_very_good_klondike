/* gamelog_cmd.cpp
 * contains the implementation of the GameLogCmd
 * @kemcbride/ke2mcbri 2025
 */

#include "gamelog_cmd.h"

using namespace std;

GameLogCmd::HintCmd(string str) : cmd(str) {}

const string GameLogCmd::toString() const { return this->cmd; }

