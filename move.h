#ifndef MOVE_H
#define MOVE_H

#include <iostream>
#include <string>

#include "run.h"
#include "board.h"
#include "move_cmd.h"

class Move {
  private:
    Move() = delete;
    Run srcRun;
    Source srcLoc;
    Run dstRun;
    Dest dstLoc;
    unsigned int count;

    
  public:
    Move(Board, MoveCmd);
    Move(Board, Source, Dest, unsigned int);
    Move(Run, Source, Run, Dest);

    bool isLegal();
    Source getSrc();
    Dest getDst();
    Run getSrcRun();
    Run getDstRun();

};
#endif // MOVE_H
