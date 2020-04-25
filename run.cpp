/* run.cpp 
 * contains some stuff to define "runs" for solitaire play.
 * klondike specfiically, for now.
 * @kemcbride/@ke2mcbri 2020
 */

#include "run.h"

using namespace std;

#define GREENSTR string("\033[32m")
#define RESETSTR string("\033[0m")

Run::Run(Card c) {
  vector<Card> my_cards;
  my_cards.push_back(c);
  this->cards = my_cards;
}

Run::Run(std::vector<Card> cards) {
  // TODO: must validate that this set of cards satisfies "run" constarints
  if (false) throw runtime_error("doesnt satisfy run constraints");
  this->cards = cards;
}

Run::Run(Run r, unsigned int i) {
  Run tmp = r.take(i);
  this->cards = tmp.cards;
  this->revealed = tmp.isRevealed();
}

bool Run::isRevealed() {
  return this->revealed;
}

vector<Card> Run::view() {
  if (this->isRevealed())
    return this->cards;

  vector<Card> empty_none_revealed;
  return empty_none_revealed;
}

optional<Card> Run::peek() {
  if (this->cards.size() == 0)
    return nullopt;
  return this->cards.back();
}

Run Run::take(unsigned int n) {
  if (n > this->cards.size())
    n = this->cards.size(); // I'll only give you up to what I can

  auto start = this->cards.cend() - n;
  auto end = this->cards.cend();

  vector<Card> new_run(start, end);
  this->cards.erase(start, end);
  return new_run;
}

void Run::put(Run r) {
  // If we're adding a run, we know it already internally satsfies run constraints
  // so we only need to check that we can add it. (or its top card)
  if (!this->canAdd(r))
    throw runtime_error("doesnt satisfy run constraints");

  for(auto c : r.cards) {
    this->cards.push_back(c);
  }
}

void Run::put(vector<Card> cards) {
  for(auto c : cards) {
    this->put(c);
  }
}

void Run::put(Card c) {
  if (!this->canAdd(c))
    throw runtime_error("doesnt satisfy run constraints");

  this->cards.push_back(c);
}

void Run::reveal() {
  this->revealed = true;
}

// NOTE: This function is likely completely unnecessary.
void Run::hide() {
  this->revealed = false;
}

bool Run::canAdd(Card c) {
  Card myCard = this->cards.back();
  if (myCard.getColor() != c.getColor() && myCard.getRank() == c.getRank() + 1) {
    return true;
  }
  return false;
}

bool Run::canAdd(Run r) {
  Card your_first_card = r.cards.front();
  return this->canAdd(your_first_card);
}

string Run::toString() {
  string card_str;
  if (this->isRevealed()) {
    for (unsigned int i = 0; i < this->cards.size(); ++i) {
      card_str += cards.at(i).toString();
      if (!(i == this->cards.size() - 1)) card_str += ",";
    }
  } else {
    card_str = GREENSTR + "X" + RESETSTR;
  }
  return "[" + card_str + "]";
}
