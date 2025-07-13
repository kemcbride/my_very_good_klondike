#ifndef LOCATION_H
#define LOCATION_H

#include <string>

class Location {
 private:
  Location() = delete;

 public:
  int idx = 0;  // 0-offset index into list of piles or foundations
  char type = 'p';
  Location(char, int);
  std::string toString();
};

bool operator==(Location a, Location b);

inline bool operator<(const Location& lhs, const Location& rhs) {
  if (lhs.type == rhs.type) {
    return lhs.idx < rhs.idx;
  }
  return lhs.type < rhs.type;
}

class Source : public Location {
 private:
  Source() = delete;

 public:
  Source(char, int);
};

class Dest : public Location {
 private:
  Dest() = delete;

 public:
  Dest(char, int);
};

#endif  // LOCATION_H
