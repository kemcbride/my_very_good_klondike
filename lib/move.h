#ifndef MOVE_H
#define MOVE_H

#include <iostream>
#include <string>

#include "run.h"
/* #include "board.h" */
#include "move_cmd.h"

class Move {
private:
  Move() = delete;
  Run srcRun;
  Source srcLoc;
  Run dstRun;
  Dest dstLoc;
  unsigned int count = 0;

public:
  Move(Run, Source, Run, Dest, unsigned int);

  Source getSrc();
  Dest getDst();
  unsigned int getCount();
  Run getSrcRun();
  Run getDstRun();
};
#endif // MOVE_H
