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
#include "pile.h"
#include "stock.h"


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
