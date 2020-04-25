/* stock.cpp 
 * contains some stuff to define stock for solitaire play.
 * klondike specfiically, for now.
 * @kemcbride/@ke2mcbri 2020
 */

#include "stock.h"

using namespace std;

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

string Stock::toString() {
  string card_str;
  if (this->peek().has_value()) {
    card_str = this->peek().value().toString();
  } else {
    card_str = ' ';
  }
  return "<" + card_str + ">";
}
