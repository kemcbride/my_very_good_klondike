/* foundation.cpp 
 * contains some stuff to define "foundation" for solitaire play.
 * klondike specfiically, for now.
 * @kemcbride/@ke2mcbri 2020
 */

#include "foundation.h"

using namespace std;

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
