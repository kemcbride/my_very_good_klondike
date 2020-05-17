

#include "move.h"

using namespace std;

Move::Move(Board b, MoveCmd m_cmd) : srcLoc(m_cmd.getSource()), dstLoc(m_cmd.getDest()) {
  this->srcRun = b.getSourceRun(m_cmd.getSource(), m_cmd.getCount());
  this->dstRun = b.getDestRun(m_cmd.getDest());
}

Move::Move(Board b, Source s, Dest d, unsigned int count) : srcLoc(s), dstLoc(d) {
  this->srcRun = b.getSourceRun(s, count);
  this->dstRun = b.getDestRun(d);
}
Move::Move(Run s, Source src, Run d, Dest dst) : srcRun(s), srcLoc(src), dstRun(d), dstLoc(dst) {}

Source Move::getSrc() {
  return this->srcLoc;
}

Run Move::getSrcRun() {
  return this->srcRun;
}

Dest Move::getDst() {
  return this->dstLoc;
}
Run Move::getDstRun() {
  return this->dstRun;
}

bool Move::isLegal() {
  return this->dstRun.canAdd(this->srcRun);
}
