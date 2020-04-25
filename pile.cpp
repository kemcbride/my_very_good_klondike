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

Pile::Pile(Deck &d, int n) {
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

void Pile::clear_empty_run() {
  if (this->runs.back().cards.size() == 0) {
    this->runs.pop_back(); // remove empty run

    if (this->runs.size() > 0) {
      this->runs.back().reveal(); // reveal new front
    }
  }
}

optional<Run> Pile::pop() {
  if (this->runs.size() == 0)
    return nullopt;

  optional<Run> result_run = this->runs.back().take(1);
  this->clear_empty_run();
  return result_run;
}

optional<Run> Pile::take(unsigned int i) {
  optional<Run> result_run = this->runs.back().take(i);
  if (!result_run.has_value()) {
    return nullopt;
  }
  result_run.value().reveal();
  this->clear_empty_run();
  return result_run.value();
}

void Pile::put(std::vector<Card> cards) {
  this->runs.back().put(cards);
}

void Pile::put(Run run) {
  this->runs.back().put(run);
}


string Pile::toString() {
  string runs_str;
  for (auto r : this->runs) {
    runs_str += r.toString();
  }
  return runs_str;
}
