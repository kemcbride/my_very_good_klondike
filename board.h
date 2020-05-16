#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include <optional>

#include "deck.h"
#include "foundation.h"
#include "pile.h"
#include "stock.h"
#include "tableau.h"
#include "move_cmd.h"


class Board {
  private:
    bool is_solved = false;
    bool is_stuck = false;
    bool show_labels = false;
    bool isSolved(); // check if the game is "Solved"/"Won"
    bool isStuck(); // check if the game is "Stuck"/"Lost"
    bool isLegal(MoveCmd);
    std::vector<Source> getAllSources();
    std::vector<Dest> getAllDests();
    std::vector<int> getAllCounts(Run); // return list of possible move sizes
    std::vector<MoveCmd> allPossibleMoves();
    std::vector<MoveCmd> allLegalMoves();

    void reveal_top_runs();

  public:
    Tableau tableau;
    Stock stock;
    std::vector<Foundation> foundations;

    Board(Deck&);
    std::string toString();

    void toggle_labels();
    void next();

    void move(MoveCmd);
};

#endif // BOARD_H
