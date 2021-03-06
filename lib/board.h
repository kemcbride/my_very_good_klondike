#ifndef BOARD_H
#define BOARD_H

#include <chrono>
#include <optional>
#include <set>
#include <vector>
#include <iomanip>
#include <sstream>

#include "deck.h"
#include "foundation.h"
#include "location.h"
#include "move.h"
#include "command.h"
#include "move_cmd.h"
#include "pile.h"
#include "run.h"
#include "stock.h"
#include "tableau.h"

class Board {
private:
  bool is_solved = false;
  bool is_stuck = false;
  bool show_labels = false;
  std::vector<Move> legal_moves;
  std::vector<std::string> legal_commands;
  int hint_idx = 0;
  std::chrono::time_point<std::chrono::system_clock> game_start;
  std::chrono::time_point<std::chrono::system_clock> game_end;

  bool isSolved(); // check if the game is "Solved"/"Won"
  bool isStuck();  // check if the game is "Stuck"/"Lost"
  std::vector<Source> getAllSourcesButStock();
  std::vector<Dest> getAllDests();
  std::vector<int> getAllCounts(Run); // return list of possible move sizes
  std::vector<Move> allPossibleMoves();
  std::vector<Move> allLegalMoves();
  std::vector<std::string> allLegalCommands();

  void reveal_top_runs();

public:
  Tableau tableau;
  Stock stock;
  std::vector<Foundation> foundations;

  Board(Deck &);
  std::string toString();

  void toggle_labels();
  void next();
  std::string hint();

  bool isLegal(Move);
  bool isMeaningful(Move);
  void move(Move);
  void move(MoveCmd);

  Run getSourceRun(Source, unsigned int); // returns count-sized run (or less)
  Run getDestRun(Dest);                   // returns empty or 1-card run
};

#endif // BOARD_H
