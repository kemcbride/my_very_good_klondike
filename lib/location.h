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

  friend bool operator==(const Location& lhs, const Location& rhs) = default;
};

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
  friend bool operator==(const Source& lhs, const Source& rhs) = default;
};

class Dest : public Location {
 private:
  Dest() = delete;

 public:
  Dest(char, int);
  friend bool operator==(const Dest& lhs, const Dest& rhs) = default;
};

class LocPair {
 private:
  LocPair() = delete;

 public:
  LocPair(Source, Dest);
  Source src;
  Dest dst;
  std::string toString();
  inline friend bool operator<(const LocPair& lhs, const LocPair& rhs) {
    if (lhs.src == rhs.src) {
      return lhs.dst < rhs.dst;
    }
    return lhs.src < rhs.src;
  }
};

#endif  // LOCATION_H
