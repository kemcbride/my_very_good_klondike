#ifndef STOCK_H
#define STOCK_H

#include <optional>
#include <vector>

#include "card.h"
#include "deck.h"

// The stock in my world is a circular buffer that can pop elements
class Stock {
private:
  unsigned int index;
  bool update_index(); // did we have to reset to 0?

public:
  Stock() = delete;
  std::vector<Card> cards;

  Stock(Deck);

  std::optional<Card> peek();
  std::optional<Card> pop();
  bool next(); // returns: did this move recycle the stock?

  std::string toString();
};

#endif
