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
  if (this->runs.back().cards.size() == 0)
    this->runs.pop_back(); // remove empty run
}

optional<Run> Pile::pop() {
  if (this->runs.size() == 0)
    return nullopt;

  optional<Run> result_run = this->runs.back().take(1);
  this->clear_empty_run();
  return result_run;
}

optional<Run> Pile::peek(unsigned int n) {
  if (this->runs.size() == 0 || n == 0)
    return nullopt;
  vector<Card> run_cards = this->runs.back().cards;
  if (run_cards.size() < n) {
    return run_cards;
  }
  // return the back N cards from the view
  unsigned int start_offset = run_cards.size() - n;
  vector<Card> peeked_cards(run_cards.begin() + start_offset, run_cards.end());
  Run peeked_run(peeked_cards);
  return peeked_run;
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
  if (this->runs.empty()) {
    Card c = cards.front();
    if (c.getRank() != 13)
      throw runtime_error("Only Kings can be placed on an empty pile");

    Run r(cards);
    r.reveal();
    this->runs.push_back(r);
  } else {
    this->runs.back().put(cards);
  }
}

void Pile::put(Card c) {
  if (this->runs.empty()) {
    if (c.getRank() != 13)
      throw runtime_error("Only Kings can be placed on an empty pile");
    Run r(c);
    r.reveal();
    this->runs.push_back(r);
  } else {
    this->runs.back().put(c);
  }
}

void Pile::put(Run run) {
  if (this->runs.empty()) {
    if (run.cards.empty()) {
      throw runtime_error("Pile:put: don't put empty runs");
    }
    Card c = run.cards.front();
    if (c.getRank() != 13)
      throw runtime_error("Only Kings can be placed on an empty pile");
    run.reveal();
    this->runs.push_back(run);
  } else {
    if (!this->runs.back().isRevealed()) {
      // then we can put anything, we were in the middle of a move and it failed
      this->runs.push_back(run);
    } else {
      this->runs.back().put(run);
    }
  }
}

void Pile::reveal_top() {
  if (!this->runs.empty())
    this->runs.back().reveal();
}

string Pile::toString() {
  string runs_str;
  for (auto r : this->runs) {
    runs_str += r.toString();
  }
  return runs_str;
}
