/* deck.cpp 
 * contains some stuff to define decks for solitaire play.
 * klondike specfiically, for now.
 * @kemcbride/@ke2mcbri 2020
 */

#include "deck.h"

using namespace std;


Deck::Deck() {
  this->sort(); // sets up the ->cards
  this->current_idx = 0;
}

void Deck::shuffle() {
  random_shuffle(this->cards.begin(), this->cards.end());
}

void Deck::sort() {
  // Assuming that the deck always contains the same 52 cards...
  vector<Card> sorted_deck;
  for (int i=0; i < 4; ++i) { // suit
    for (int j=1; j < 14; ++j) { // rank
      sorted_deck.push_back(Card(j, i));
    }
  }
  this->cards = sorted_deck;
}

vector<Card> Deck::draw(int n) {
  int whats_left = this->cards.size() - this->current_idx;
  int deal_size = min(whats_left, n);

  auto first = this->cards.cbegin() + this->current_idx;
  auto last = this->cards.cbegin() + this->current_idx + deal_size;

  vector<Card> your_draw(first, last);

  this->current_idx += deal_size;
  return your_draw;
}

void Deck::reset_index() {
  this->current_idx = 0;
}
