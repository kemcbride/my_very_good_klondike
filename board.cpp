/* board.cpp 
 * contains some stuff to define board for solitaire play.
 * klondike specfiically, for now.
 * @kemcbride/@ke2mcbri 2020
 */

#include "board.h"

using namespace std;

// Pile - begin
Pile::Pile(vector<Card> deal) {
  vector<Run> myruns;
  // Initialize 1-card runs
  for (auto c : deal) {
    myruns.push_back(Run(c));
  }
  this->runs = myruns;
  this->runs.back().reveal();
}

Pile::Pile(vector<Run> runs) {
  this->runs = runs;
  for (auto r : runs) {
    r.hide();
  }
  this->runs.back().reveal();
}

Pile::Pile(Deck d, int n) {
  vector<Run> myruns;

  // Initialize 1-card runs
  for (auto c : d.draw(n)) {
    myruns.push_back(Run(c));
  }
  this->runs = myruns;
  this->runs.back().reveal();
}
// Pile - end
