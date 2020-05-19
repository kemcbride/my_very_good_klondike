

#include "move.h"

using namespace std;

Move::Move(Run s, Source src, Run d, Dest dst, unsigned int i)
    : srcRun(s), srcLoc(src), dstRun(d), dstLoc(dst), count(i) {}

Source Move::getSrc() { return this->srcLoc; }

Run Move::getSrcRun() { return this->srcRun; }

Dest Move::getDst() { return this->dstLoc; }
Run Move::getDstRun() { return this->dstRun; }

unsigned int Move::getCount() { return this->count; }
