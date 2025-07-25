#ifndef DECK_H
#define DECK_H

#include <algorithm>  // random_shuffle
#include <random>     // random_device / mt19937
#include <vector>

#include "card.h"

class Deck {
 private:
  int current_idx;
  std::mt19937 generator;

 public:
  std::vector<Card> cards;

  Deck(std::mt19937 generator);
  void shuffle();
  void sort();

  // Draw a subset of the deck, from "current idx"
  std::vector<Card> draw(int);
  // Reset the draw index to the "top" of the deck (0)
  void reset_index();
  void update_seed(int);
};

#endif  // DECK_H
