#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include <optional>

#include "deck.h"
#include "foundation.h"
#include "pile.h"
#include "run.h"
#include "stock.h"
#include "tableau.h"
#include "move_cmd.h"
#include "move.h"
#include "location.h"


class Board {
  private:
    bool is_solved = false;
    bool is_stuck = false;
    bool show_labels = false;
    bool isSolved(); // check if the game is "Solved"/"Won"
    // IN PROGRESS
    bool isStuck(); // check if the game is "Stuck"/"Lost"
    std::vector<Source> getAllSourcesButStock();
    std::vector<Dest> getAllDests();
    std::vector<int> getAllCounts(Run); // return list of possible move sizes
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

    bool isLegal(Move);
    void move(Move);
    void move(MoveCmd);

    Run getSourceRun(Source, unsigned int); // returns count-sized run (or less)
    Run getDestRun(Dest); // returns empty or 1-card run
};

#endif // BOARD_H
