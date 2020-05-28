/* hint_cmd.cpp
 * like, contains the implementation of the HintCmd object.
 * @kemcbride/ke2mcbri 2020 :o
 */

#include "hint_cmd.h"

using namespace std;

HintCmd::HintCmd(string str) : cmd(str) {}

HintCmd::HintCmd(Move m) : cmd(m.toString()) {}

string HintCmd::toString() { return this->cmd; }
