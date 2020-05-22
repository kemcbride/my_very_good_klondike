

#include "move.h"

using namespace std;

Move::Move(Run s, Source src, Run d, Dest dst, unsigned int i)
    : srcRun(s), srcLoc(src), dstRun(d), dstLoc(dst), count(i) {}

Source Move::getSrc() { return this->srcLoc; }

Run Move::getSrcRun() { return this->srcRun; }

Dest Move::getDst() { return this->dstLoc; }
Run Move::getDstRun() { return this->dstRun; }

unsigned int Move::getCount() { return this->count; }

string Move::toString() {
  string src = this->getSrc().toString();
  string dst = this->getDst().toString();
  string cnt = to_string(this->getCount());
  return src + " " + cnt + " " + dst;
}

const bool operator<(Move a, Move b) {
  Source as = a.getSrc();
  Source bs = b.getSrc();
  Dest ad = a.getDst();
  Dest bd = b.getDst();
  return ( as.type < bs.type || as.idx < bs.idx ||
      ad.type < bd.type || ad.idx < bd.idx ||
      a.getCount() < b.getCount()
  );
}
