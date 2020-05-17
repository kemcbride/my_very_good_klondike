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

Source::Source(char c, int i) : Location(c, i) {}

Dest::Dest(char c, int i) : Location(c, i) {}
