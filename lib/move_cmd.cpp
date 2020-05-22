/* move.cpp
 * like, contains the implementation of the MoveCmd object.
 * cause you play solitaire by making moves
 * @kemcbride/ke2mcbri 2020 :o
 */

#include "move_cmd.h"

using namespace std;

int char_to_int(char c) { return c - '0'; }

MoveCmd::MoveCmd(string str)
    : cmd(str), source(this->parseSource(str)), dest(this->parseDest(str)),
      count(this->parseCount(str)) {}

MoveCmd::MoveCmd(char s, int s_idx, char d, int d_idx, int count)
    : source(s, s_idx), dest(d, d_idx) {}

MoveCmd::MoveCmd(Source s, Dest d, int c) : source(s), dest(d), count(c) {}

Source MoveCmd::getSource() { return this->source; }

Dest MoveCmd::getDest() { return this->dest; }

int MoveCmd::getCount() { return this->count; }

// All of the parse* funcs can throw. So Watch the heck out
Source MoveCmd::parseSource(string str) {
  string source = string(str, 0, str.find(" "));
  // Default values:
  char type = 's';
  int idx = 0;

  if (source[0] == 'p') { // pile
    if (!(this->validatePile(source))) {
      throw runtime_error("Invalid source: " + source);
    }
    type = 'p';
    idx = char_to_int(source[1]) - 1;
  } else if (source[0] == 'f') { // foundation
    if (!this->validateFdn(source)) {
      throw runtime_error("Invalid source: " + source);
    }
    type = 'f';
    idx = char_to_int(source[1]) - 1;
  } else { // stock
    type = 's';
  }
  return Source(type, idx);
}

Dest MoveCmd::parseDest(string str) {
  string dest = string(str, str.rfind(" ") + 1, str.size());
  // Default values:
  char type = 'f';
  int idx = 1;

  if (dest[0] == 'p') {
    // pile
    if (!this->validatePile(dest)) {
      throw runtime_error("Invalid dest: " + dest);
    }
    type = 'p';
    idx = char_to_int(dest[1]) - 1;
  } else if (dest[0] == 'f') {
    // foundation
    if (!this->validateFdn(dest)) {
      throw runtime_error("Invalid dest: " + dest);
    }
    type = 'f';
    idx = char_to_int(dest[1]) - 1;
  } else {
    // anything else is actually invalid. Should I throw here? yes.
    throw runtime_error("Invalid dest: " + dest);
  }
  return Dest(type, idx);
}

int MoveCmd::parseCount(string str) {
  string second_half = string(str, str.find(" ") + 1, str.size());
  string count_part = string(second_half, 0, str.find(" "));
  // if we call this in initializer list; must support non-p answeers
  if (str[0] != 'p') {
    return 0;
  }

  int c = stoi(count_part);
  if (c < 0 || c > 13) {
    throw runtime_error("Invalid count: " + count_part);
  }
  return c;
}

bool MoveCmd::validateFdn(string s) {
  if (s[0] == 'f') {
    // From the string (1-offset) to 0-offset
    int f = char_to_int(s[1]) - 1;
    if (f >= 0 && f < 4) {
      return true;
    }
  }
  return false;
};

bool MoveCmd::validatePile(string s) {
  if (s[0] == 'p') {
    // From the string (1-offset) to 0-offset
    int p = char_to_int(s[1]) - 1;
    if (p >= 0 && p < 7) {
      return true;
    }
  }
  return false;
};

string MoveCmd::toString() {
  string src = this->getSource().toString();
  string dst = this->getDest().toString();
  string cnt = to_string(this->getCount());
  return src + " " + cnt + " " + dst;
}
