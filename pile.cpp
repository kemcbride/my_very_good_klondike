/* pile.cpp 
 * contains some stuff to define pile for klondike solitaire
 * @kemcbride/@ke2mcbri 2020
 */

#include "pile.h"

using namespace std;

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

  // Initialize 1-card hidden runs
  for (auto c : d.draw(n)) {
    myruns.push_back(Run(c));
  }
  this->runs = myruns;
  this->runs.back().reveal();
}

optional<Card> Pile::peek() {
  if (this->runs.size() == 0)
    return nullopt;
  return this->runs.back().peek();
}
