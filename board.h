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
#include "foundation.h"
#include "run.h"


// a Pile is a stack of cards that you can push and pop from.
// However, it works wayy differently than a foundation.
// The pile is probably where the business logic of what moves
// are viable lives.
class Pile {
  public:
    std::vector<Run> runs;

    // TODO: Not sure which I want to use...
    Pile(std::vector<Card>);
    Pile(std::vector<Run>);
    Pile(Deck, int);

    std::optional<Card> peek();
    std::optional<Card> pop(); // remove the top card
    Run take(unsigned int); // take the top N visible cards (if they are movable)
    void put(std::vector<Card>); // add a stack of viable cards to the top of the pile
};

// The Tableau consists of 7 piles
class Tableau {
  public:
    std::vector<Pile> piles;

    Tableau(Deck);
};

class Board {
  private:
    bool isSolved(); // check if the game is "Solved"/"Solvable"

  public:
    Tableau tableau;
    Stock stock;
    std::vector<Foundation> foundations;

    Board(Deck);
};

#endif // BOARD_H
