#ifndef MOVE_H
#define MOVE_H

#include <iostream>
#include <string>

#include "run.h"
/* #include "board.h" */
#include "move_cmd.h"

using namespace solitaire;

enum MoveType {
  STK2FDN,
  STK2TBL,
  TBL2FDN,
  FDN2TBL,
  TBL2TBL,
  REVEAL,
  RECYCLE,
  UNKNOWN
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
  Move(const Move& m) : srcRun(m.getSrcRun()), srcLoc(m.getSrc()), dstRun(m.getDstRun()), dstLoc(m.getDst()), count(m.getCount()) {};
  Move(Move&& m) noexcept : srcRun(m.getSrcRun()), srcLoc(m.getSrc()), dstRun(m.getDstRun()), dstLoc(m.getDst()), count(m.getCount()) {};
  MoveCmd toMoveCmd();

  const Source getSrc() const;
  const Dest getDst() const;
  const unsigned int getCount() const;
  const MoveType getMoveType() const;
  const Run getSrcRun() const;
  const Run getDstRun() const;

  const std::string toString() const;
};

// To support set<Move>
bool operator<(Move, Move);
#endif  // MOVE_H
