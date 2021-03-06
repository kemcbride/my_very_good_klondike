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
  void update_index();

public:
  Stock() = delete;
  std::vector<Card> cards;

  Stock(Deck);

  std::optional<Card> peek();
  std::optional<Card> pop();
  void next();

  std::string toString();
};

#endif
