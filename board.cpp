/* board.cpp 
 * contains some stuff to define board for solitaire play.
 * klondike specfiically, for now.
 * @kemcbride/@ke2mcbri 2020
 */

#include "board.h"

using namespace std;


// Run - begin
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

bool Run::isRevealed() {
  return this->revealed;
}

vector<Card> Run::view() {
  if (this->isRevealed())
    return this->cards;

  vector<Card> empty_none_revealed;
  return empty_none_revealed;
}

Run Run::take(int n) {
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
  int myRank = static_cast<int>(myCard.getRank());
  if (myCard.getColor() != c.getColor() && myCard.getRank() == c.getRank() + 1) {
    return true;
  }
  return false;
}

bool Run::canAdd(Run r) {
  // TODO: we need ordering for cards? and we need to easily check suit difference
  Card your_first_card = r.cards.front();
  return this->canAdd(your_first_card);
}
// Run - end

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


// Stock - begin
Stock::Stock(Deck d) {
  // Hmm, the stock has to take all remaining cards in the deck.
  // I think I'll do that by drawing n = 52
  this->cards = d.draw(52);
  this->index = 0;
}

void Stock::update_index() {
  // So, let's say we just called next() or pop() - we've
  // changed the contents/state of ->cards and ->index somehow,
  // and we want to make sure it's still valid.

  // -> Maybe erase does all the work for me?
  // http://www.cplusplus.com/reference/vector/vector/erase/
  if (this->index >= this->cards.size()) {
    this->index = 0;
  }
}

optional<Card> Stock::peek() {
  if (this->cards.size() == 0) {
    return nullopt;
  }
  return this->cards.at(this->index);
}

optional<Card> Stock::pop() {
  if (this->cards.size() == 0) {
    return nullopt;
  }
  Card c = this->cards.at(this->index);
  this->cards.erase(this->cards.cbegin() + this->index);
  this->update_index();
  return c;
}

void Stock::next() {
  ++(this->index);
  this->update_index(); // reset it to 0 if necessary
}
// Stock - end


// Foundation - begin
Foundation::Foundation() {}; // default - nothing needs to be set

optional<Card> Foundation::peek() {
  if (this->cards.size() == 0) {
    return nullopt;
  }
  return this->cards.back();
}

optional<Card> Foundation::pop() {
  if (this->cards.size() == 0) {
    return nullopt;
  }
  Card c = this->cards.back();
  this->cards.pop_back();
  return c;
}

void Foundation::push(Card c) {
  //TODO: make this only accept ALLOWED pushes
  this->cards.push_back(c);
}
// Foundation - end
