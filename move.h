#ifndef MOVE_H
#define MOVE_H

#include <iostream>
#include <string>

#include "run.h"

class Move {
  private:
    Move() = delete;
    Run source;
    Run dest;

    
  public:
    Move(Run, Run);

    bool isLegal();
    Run getSource();
    Run getDest();

};
#endif // MOVE_H
