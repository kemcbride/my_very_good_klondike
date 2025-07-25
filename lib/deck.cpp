/* deck.cpp
 * contains some stuff to define decks for solitaire play.
 * klondike specfiically, for now.
 * @kemcbride/@ke2mcbri 2020
 */

#include "deck.h"

using namespace std;

Deck::Deck(mt19937 g) {
  sort();  // sets up the ->cards
  current_idx = 0;
  generator = g;
}

void Deck::shuffle() { ::shuffle(cards.begin(), cards.end(), generator); }

void Deck::sort() {
  // Assuming that the deck always contains the same 52 cards...
  vector<Card> sorted_deck;
  for (int i = 0; i < 4; ++i) {     // suit
    for (int j = 1; j < 14; ++j) {  // rank
      sorted_deck.push_back(Card(j, i));
    }
  }
  cards = sorted_deck;
}

vector<Card> Deck::draw(int n) {
  int whats_left = cards.size() - current_idx;
  int deal_size = min(whats_left, n);

  auto first = cards.cbegin() + current_idx;
  auto last = cards.cbegin() + current_idx + deal_size;

  vector<Card> your_draw(first, last);

  current_idx += deal_size;
  return your_draw;
}

void Deck::reset_index() { current_idx = 0; }

void Deck::update_seed(int seed) { generator = std::mt19937(seed); }
