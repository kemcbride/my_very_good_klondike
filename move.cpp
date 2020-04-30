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

Move::Move(string str) {
  // We have to parse out the source, count, and dest
  // from the input string.
  // inputs like:
  // s f1
  // s p2
  // s 2 p2 (invalid)
  // f2 2 p1 (invalid)
  // f1 p4
  // f3 s (invalid)
  // p5 3 p3
  // p2 3 f3 (invalid)
  // p1 3 s (invalid)
  this->cmd = str;
  this->source = this->parseSource(str);
  this->dest = this->parseDest(str);
  if (this->source.type == 'p') {
    this->count = this->parseCount(str);
  }
}

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
  Source src;

  if(source[0] == 'p') {
    // pile
    if( !(this->validatePile(source)) ){
      throw runtime_error("Invalid source: " + source);
    }
    src.type = 'p';
    src.idx = char_to_int(source[1]);
  } else if (source[0] == 's') {
    // stock
    src.type = 's';
  } else if (source[0] == 'f') {
    // foundation
    if( !this->validateFdn(source) ){
      throw runtime_error("Invalid source: " + source);
    }
    src.type = 'f';
    src.idx = char_to_int(source[1]);
  }
  return src;
}

Dest Move::parseDest(string str) {
  string dest = string(str, str.rfind(" ")+1, str.size());
  Dest dst;

  if(dest[0] == 'p') {
    // pile
    if( !this->validatePile(dest) ){
      throw runtime_error("Invalid dest: " + dest);
    }
    dst.type = 'p';
    dst.idx = char_to_int(dest[1]);
  } else if (dest[0] == 's') {
    // stock
    dst.type = 's';
  } else if (dest[0] == 'f') {
    // foundation
    if( !this->validateFdn(dest) ){
      throw runtime_error("Invalid dest: " + dest);
    }
    dst.type = 'f';
    dst.idx = char_to_int(dest[1]);
  }
  return dst;
}

int Move::parseCount(string str) {
  string second_half = string(str, str.find(" ")+1, str.size());
  string count_part = string(second_half, 0, str.find(" "));

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

