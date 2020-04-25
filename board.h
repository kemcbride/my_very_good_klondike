#ifndef BOARD_H
#define BOARD_H

#include <vector>

#include "deck.h"
#include "foundation.h"
#include "pile.h"
#include "stock.h"
#include "tableau.h"


class Board {
  private:
    bool isSolved(); // check if the game is "Solved"/"Solvable"

  public:
    Tableau tableau;
    Stock stock;
    std::vector<Foundation> foundations;

    Board(Deck&);
    Board(Tableau, Stock);
    std::string toString();
};

#endif // BOARD_H
