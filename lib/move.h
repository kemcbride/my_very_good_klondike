#ifndef MOVE_H
#define MOVE_H

#include <iostream>
#include <string>

#include "run.h"
/* #include "board.h" */
#include "move_cmd.h"

enum MoveType {
  STK2FDN, STK2TBL, TBL2FDN, FDN2TBL, TBL2TBL, REVEAL, RECYCLE, UNKNOWN
};

class Move {
private:
  Move() = delete;
  Run srcRun;
  Source srcLoc;
  Run dstRun;
  Dest dstLoc;
  MoveType moveType;
  unsigned int count = 0;

public:
  Move(Run, Source, Run, Dest, unsigned int);
  MoveCmd toMoveCmd();

  Source getSrc();
  Dest getDst();
  unsigned int getCount();
  MoveType getMoveType();
  Run getSrcRun();
  Run getDstRun();

  std::string toString();
};

// To support set<Move>
const bool operator<(Move, Move);
#endif // MOVE_H
