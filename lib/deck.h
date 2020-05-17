#ifndef DECK_H
#define DECK_H

#include <algorithm> // random_shuffle
#include <vector>

#include "card.h"

class Deck {
private:
  int current_idx;

public:
  std::vector<Card> cards;

  Deck();
  void shuffle();
  void sort();

  // Draw a subset of the deck, from "current idx"
  std::vector<Card> draw(int);
  // Reset the draw index to the "top" of the deck (0)
  void reset_index();
};

#endif // DECK_H
