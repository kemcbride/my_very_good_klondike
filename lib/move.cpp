/* move.cpp
 * like, contains the implementation of the Move object.
 * cause you play solitaire by making moves
 * @kemcbride/ke2mcbri 2020 :o
 */

#include "move.h"

using namespace std;
using namespace solitaire;

MoveType moveTypeFromSrcDst(Source s, Dest d) {
  switch (s.type) {
    case 'p':
      switch (d.type) {
        case 'p':
          return TBL2TBL;
        case 'f':
          return TBL2FDN;
        default:  // unknown - shouldn't be reachable
          return UNKNOWN;
      }
    case 's':
      switch (d.type) {
        case 'p':
          return STK2TBL;
        case 'f':
          return STK2FDN;
        default:  // shouldn't be reachable
          return UNKNOWN;
      }
    case 'f':
      switch (d.type) {
        case 'p':
          return FDN2TBL;
        default:  // shouldn't be reachable
          return UNKNOWN;
      }
  }
  return UNKNOWN;
}

Move::Move(Run s, Source src, Run d, Dest dst, unsigned int i)
    : srcRun(s), srcLoc(src), dstRun(d), dstLoc(dst), count(i) {
  this->moveType = moveTypeFromSrcDst(src, dst);
}

MoveCmd Move::toMoveCmd() {
  return MoveCmd(this->srcLoc, this->dstLoc, this->count);
}

const Source Move::getSrc() const { return this->srcLoc; }

const Run Move::getSrcRun() const { return this->srcRun; }

const Dest Move::getDst() const { return this->dstLoc; }
const Run Move::getDstRun() const { return this->dstRun; }

const unsigned int Move::getCount() const { return this->count; }
const MoveType Move::getMoveType() const { return this->moveType; }

const string Move::toString() const {
  string src = this->getSrc().toString();
  string dst = this->getDst().toString();
  string cnt = to_string(this->getCount());
  if (src == "s") {
    return src + " " + dst;
  }
  return src + " " + cnt + " " + dst;
}

bool operator<(Move a, Move b) {
  Source as = a.getSrc();
  Source bs = b.getSrc();
  Dest ad = a.getDst();
  Dest bd = b.getDst();
  return (as.type < bs.type || as.idx < bs.idx || ad.type < bd.type ||
          ad.idx < bd.idx || a.getCount() < b.getCount());
}
