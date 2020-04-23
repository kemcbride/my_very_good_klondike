#ifndef BOARD_H
#define BOARD_H

#include <algorithm> // random_shuffle
#include <iostream>
#include <string>
#include <exception>
#include <optional>
#include <vector>

#include "deck.h"
#include "card.h"

// a Pile is a stack of cards that you can push and pop from.
// However, it works wayy differently than a foundation.
// The pile is probably where the business logic of what moves
// are viable lives.
class Pile {
  public:
    std::vector<Card> cards;

    Pile(std::vector<Card>);
    std::optional<Card> peek();
    std::optional<Card> pop(); // remove the top card
    std::vector<Card> take(int); // take the top N visible cards (if they are movable)
    void put(std::vector<Card>); // add a stack of viable cards to the top of the pile
};

// The Tableau consists of 7 piles
class Tableau {
  public:
    std::vector<Pile> piles;

    Tableau(Deck);
};

// The stock in my world is a circular buffer that can pop elements
class Stock {
  private:
    int index;

  public:
    std::vector<Card> cards;

    Stock(Deck);

    std::optional<Card> peek();
    std::optional<Card> pop();
    void next();
};

// The foundation is a ordered stack of Cards of the same suit
class Foundation {
  public:
    std::optional<Suit> suit;
    std::vector<Card> cards;

    Foundation();

    std::optional<Card> peek();
    std::optional<Card> pop();
    void push();
};

class Board {
  public:
    Tableau tableau;
    Stock stock;
    std::vector<Foundation> foundations;

    Board(Deck);
};

#endif // BOARD_H
