#ifndef MOVE_H
#define MOVE_H

#include <iostream>
#include <string>

// Useful helper... why is this task annoying? Who knows.
int char_to_int(char);

class Location {
  private:
    Location() = delete;
  public:
    int idx = 0; // 0-offset index into list of piles or foundations
    char type = 'p';
    Location(char, int);
};

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

class MoveCmd {
  private:
    MoveCmd() = delete;
    std::string cmd;
    Source source;
    Dest dest;
    int count = 0;

    Source parseSource(std::string);
    int parseCount(std::string);
    Dest parseDest(std::string);

    bool validateFdn(std::string);
    bool validatePile(std::string);
    
  public:
    MoveCmd(std::string);
    MoveCmd(char, int, char, int, int);
    MoveCmd(Source, Dest, int);

    Source getSource();
    Dest getDest();
    int getCount();

};

#endif // MOVE_H
