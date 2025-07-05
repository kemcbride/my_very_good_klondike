/* run.cpp
 * contains some stuff to define "runs" for solitaire play.
 * klondike specfiically, for now.
 * @kemcbride/@ke2mcbri 2020
 */

#include "run.h"

using namespace std;
using namespace solitaire;

#define GREENSTR string("\033[32m")
#define RESETSTR string("\033[0m")

Run::Run() {}

Run::Run(Card c) {
  vector<Card> my_cards;
  my_cards.push_back(c);
  this->cards = my_cards;
}

Run::Run(std::vector<Card> cards) {
  if (!cards.empty()) {
    // validate run constraints, so you're not making bogus runs
    for (auto it = cards.begin() + 1; it != cards.end(); ++it) {
      vector<Card> first_part(cards.begin(), it);
      vector<Card> second_part(it, cards.end());

      if (!Run(first_part).canAdd(Run(second_part)))
        // TODO - make this exception more detailed, list the first/second parts
        throw runtime_error(
            "run from vec<card> - doesnt satisfy run constraints");
    }
  }
  this->cards = cards;
}

Run::Run(Run r, unsigned int i) {
  Run tmp = r.take(i);
  this->cards = tmp.cards;
  this->revealed = tmp.isRevealed();
}

bool Run::isRevealed() { return this->revealed; }

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
  // If we're adding a run, we know it already internally satsfies run
  // constraints so we only need to check that we can add it. (or its top card)
  if (!this->canAdd(r))
    throw runtime_error("run.cpp: doesn't satisfy run constraints");

  for (auto c : r.cards) {
    this->cards.push_back(c);
  }
}

void Run::put(vector<Card> cards) {
  for (auto c : cards) {
    this->put(c);
  }
}

void Run::put(Card c) {
  if (!this->canAdd(c))
    throw runtime_error("doesnt satisfy run constraints");

  this->cards.push_back(c);
}

int Run::reveal() {
  int change = 0;
  if (!this->isRevealed()) change = 1;
  this->revealed = true;
  return change;
}

// NOTE: This function is likely completely unnecessary.
void Run::hide() { this->revealed = false; }

bool Run::canAdd(Card c) {
  if (this->cards.empty() && c.getRank().getValue() == 13) {
    // If I'm empty (strange case, but used for programmatically generated
    // moves) During normal game play this is handled via empty pile put, not
    // run put
    return true;
  } else if (this->cards.empty()) {
    return false;
  }

  Card myCard = this->cards.back();
  if (myCard.getColor() != c.getColor() &&
      myCard.getRank().getValue() == c.getRank().getValue() + 1) {
    return true;
  }
  return false;
}

bool Run::canAdd(Run r) {
  if (!r.cards.empty()) {
    Card your_first_card = r.cards.front();
    return this->canAdd(your_first_card);
  }
  return false;
}

string Run::toString() {
  string card_str;
  if (this->isRevealed()) {
    for (unsigned int i = 0; i < this->cards.size(); ++i) {
      card_str += cards.at(i).toString();
      if (!(i == this->cards.size() - 1))
        card_str += ",";
    }
  } else {
    card_str = GREENSTR + "X" + RESETSTR;
  }
  return "[" + card_str + "]";
}
