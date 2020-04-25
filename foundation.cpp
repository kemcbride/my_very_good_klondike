/* foundation.cpp 
 * contains some stuff to define "foundation" for solitaire play.
 * klondike specfiically, for now.
 * @kemcbride/@ke2mcbri 2020
 */

#include "foundation.h"

using namespace std;


Foundation::Foundation() {
  this->suit = nullopt;
  vector<Card> cards;
  this->cards = cards;
}

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

bool Foundation::hasSuit() {
  return this->suit.has_value();
}

Suit Foundation::getSuit() {
  if (!this->hasSuit())
    throw runtime_error("This foundation doesn't have a suit, can't getSuit()");
  return this->suit.value();
}

void Foundation::push(Card c) {
  if ( !(this->hasSuit()) ) {
    // If it's empty, we can add it and set the suit to this.
    this->cards.push_back(c);
    this->suit = c.getSuit();
    return;

  } else if (this->hasSuit() && c.getSuit() == this->getSuit()) {
    // If it's not empty, let's check suit first.
    Card top = this->peek().value();
    if (c.getRank() == top.getRank() + 1) {
      // Right suit, is it the right value? If so, we can add it.
      this->cards.push_back(c);
      return;
    }
  }
  throw runtime_error(
    "Cannot push " + c.toString() + " onto " + this->getSuit().toString() + "foundation"
  );
}

string Foundation::toString() {
  string card_str;
  if (this->peek().has_value()) {
    card_str = this->peek().value().toString();
  } else {
    card_str = "-";
  }

  // handle different str lengths (1-3)
  if (card_str.size() == 1) {
    return "( " + card_str + " )";

  } else if (card_str.size() == 2) {
    return "( " + card_str + ")";
  }
  // size == 3 (10H)
  return "(" + card_str + ")";
}
