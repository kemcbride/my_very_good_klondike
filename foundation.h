#ifndef FOUNDATION_H
#define FOUNDATION_H

#include <exception>
#include <vector>
#include <optional>

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

    bool hasSuit();
    Suit getSuit();

    std::string toString();
};

#endif // FOUNDATION_H
