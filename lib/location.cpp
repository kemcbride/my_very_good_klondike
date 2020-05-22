/* location.cpp
 * like, contains the implementation of the Location, Source, Dest objects
 * @kemcbride/ke2mcbri 2020 :o
 */

#include "location.h"

using namespace std;

Location::Location(char c, int i) {
  this->type = c;
  this->idx = i;
}

std::string Location::toString() {
  if (this->type == 's') {
    return string(1, this->type);
  }
  // We add 1 here since location objects are 0-indexed, but players use 1-indexed
  return this->type + to_string(this->idx + 1);
}

Source::Source(char c, int i) : Location(c, i) {}

Dest::Dest(char c, int i) : Location(c, i) {}
