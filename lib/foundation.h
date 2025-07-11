#ifndef FOUNDATION_H
#define FOUNDATION_H

#include <exception>
#include <optional>
#include <vector>

#include "card.h"

// The foundation is a ordered stack of Cards of the same suit
class Foundation {
 public:
  std::optional<Suit> suit;
  std::vector<Card> cards;

  Foundation();

  std::optional<Card> peek();
  std::optional<Card> pop();
  void push(Card);
  bool canPush(Card);

  bool hasSuit();
  Suit getSuit();

  bool isFull();

  std::string toString();
};

#endif  // FOUNDATION_H
