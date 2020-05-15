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
    bool show_labels = false;
    bool isSolved(); // check if the game is "Solved"/"Won"
    bool isStuck(); // check if the game is "Stuck"/"Lost"
    bool isLegal(Move);
    std::vector<Move> allPossibleMoves();
    std::vector<Move> allLegalMoves();

    void reveal_top_runs();

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
