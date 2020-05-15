/* move.cpp
 * like, contains the implementation of the Move object.
 * cause you play solitaire by making moves
 * @kemcbride/ke2mcbri 2020 :o
 */

#include "move.h"

using namespace std;

int char_to_int(char c) {
  return c - '0';
}

Location::Location(char c, int i) {
  this->type = c;
  this->idx = i;
}

Source::Source(char c, int i) : Location(c, i) {
}

Dest::Dest(char c, int i) : Location(c, i) {
}

Move::Move(string str) :
  cmd(str),
  source(this->parseSource(str)),
  dest(this->parseDest(str)),
  count(this->parseCount(str)) {
}

Move::Move(char s, int s_idx, char d, int d_idx, int count) :
  source(s, s_idx),
  dest(d, d_idx)
{}

Move::Move(Source s, Dest d, int c) : source(s), dest(d), count(c) {}

Source Move::getSource() {
  return this->source;
}

Dest Move::getDest() {
  return this->dest;
}

int Move::getCount() {
  return this->count;
}

// All of the parse* funcs can throw. So Watch the heck out
Source Move::parseSource(string str) {
  string source = string(str, 0, str.find(" "));
  char type;
  int idx;

  if(source[0] == 'p') {
    // pile
    if( !(this->validatePile(source)) ){
      throw runtime_error("Invalid source: " + source);
    }
    type = 'p';
    idx = char_to_int(source[1]);
  } else if (source[0] == 's') {
    // stock
    type = 's';
  } else if (source[0] == 'f') {
    // foundation
    if( !this->validateFdn(source) ){
      throw runtime_error("Invalid source: " + source);
    }
    type = 'f';
    idx = char_to_int(source[1]);
  }
  return Source(type, idx);
}

Dest Move::parseDest(string str) {
  string dest = string(str, str.rfind(" ")+1, str.size());
  char type;
  int idx;

  if(dest[0] == 'p') {
    // pile
    if( !this->validatePile(dest) ){
      throw runtime_error("Invalid dest: " + dest);
    }
    type = 'p';
    idx = char_to_int(dest[1]);
  } else if (dest[0] == 's') {
    // stock
    type = 's';
  } else if (dest[0] == 'f') {
    // foundation
    if( !this->validateFdn(dest) ){
      throw runtime_error("Invalid dest: " + dest);
    }
    type = 'f';
    idx = char_to_int(dest[1]);
  }
  return Dest(type, idx);
}

int Move::parseCount(string str) {
  string second_half = string(str, str.find(" ")+1, str.size());
  string count_part = string(second_half, 0, str.find(" "));
  // if we call this in initializer list; must support non-p answeers
  if (str[0] != 'p') {
    return 0;
  }

  int c = stoi(count_part);
  if ( c < 0 || c > 13) {
    throw runtime_error("Invalid count: " + count_part);
  }
  return c;
}

bool Move::validateFdn(string s) {
  if (s[0] == 'f') {
    int f = char_to_int(s[1]);
    if (f > 0 && f < 5) {
      return true;
    }
  }
  return false;
};

bool Move::validatePile(string s) {
  if (s[0] == 'p') {
    int p = char_to_int(s[1]);
    if (p > 0 && p < 8) {
      return true;
    }
  }
  return false;
};

