#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include <optional>

#include "deck.h"
#include "foundation.h"
#include "pile.h"
#include "stock.h"
#include "tableau.h"
#include "move.h"


class Board {
  private:
    bool is_solved = false;
    bool isSolved(); // check if the game is "Solved"/"Solvable"
    bool show_labels = false;

  public:
    Tableau tableau;
    Stock stock;
    std::vector<Foundation> foundations;

    Board(Deck&);
    std::string toString();

    void toggle_labels();
    void next();

    void move(Move);
};

#endif // BOARD_H
