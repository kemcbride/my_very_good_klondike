#ifndef BOARD_H
#define BOARD_H

#include <chrono>
#include <iomanip>
#include <iostream>
#include <map>
#include <numeric>
#include <optional>
#include <ranges>
#include <set>
#include <sstream>
#include <vector>

#include "command.h"
#include "deck.h"
#include "foundation.h"
#include "location.h"
#include "move.h"
#include "move_cmd.h"
#include "pile.h"
#include "run.h"
#include "stock.h"
#include "tableau.h"

#define REVEAL_VALUE 5
#define STK_2_TBL_VALUE 5
#define STK_2_FDN_VALUE 10
#define TBL_2_FDN_VALUE 10
#define FDN_2_TBL_VALUE -15
#define RECYCLE_STOCK_VALUE -100

using namespace solitaire;

class Board {
 private:
  bool is_solved = false;
  bool is_stuck = false;
  bool is_cleared = false;
  bool show_labels = false;
  bool auto_solve;  // for defaults of these, see the gflag definitions in
                    // solitaire.cpp
  bool auto_reveal;
  bool recycle_penalty_enabled;

  std::set<Dest> all_dests;
  std::vector<Move> legal_moves;
  std::vector<std::string> legal_commands;
  std::map<LocPair, std::set<Move>> _possibleMovesPerLocPair;

  size_t hint_idx = 0;
  int score = 0;
  std::chrono::milliseconds game_duration;

  std::chrono::time_point<std::chrono::system_clock> game_start;
  std::chrono::time_point<std::chrono::system_clock> game_end;

  bool foundationsFull();  // check if all foundations are "full" ie. size 13
  std::set<Source> getAllSourcesButStock();
  std::set<Dest> getAllDests();
  std::vector<int> getAllCounts(Run);  // return list of possible move sizes
  std::vector<Move> allPossibleMoves();
  std::vector<Move> allLegalMoves();
  std::vector<std::string> allLegalCommands();

  void _setupLocPairMoveMap();
  void _addMoveToPossibleMoves(LocPair lp, Move m);
  void _updatePossibleMovesMap(Move& m);
  void _updatePossibleMovesNext();
  void _updatePossibleMoves(LocPair lp);
  bool _move(Move);                     // private move execution logic
  void _move_post_processing(Move& m);  // check game state, etc.
  void reveal_top_runs();
  void solve();  // if is_solved, run all moves to clear the board.

 public:
  Tableau tableau;
  Stock stock;
  std::vector<Foundation> foundations;

  Board(Deck&, bool, bool, bool);
  std::string toString();

  void enableAutoSolve();
  void disableAutoSolve();
  void enableAutoReveal();
  void disableAutoReveal();

  void toggleLabels();
  void next();
  std::string hint();

  int getScore();
  std::chrono::milliseconds getGameDuration();

  bool isLegal(Move) const;
  bool isMeaningful(Move) const;
  void move(Move);
  void move(MoveCmd);
  bool trySolve();  // true if it has solved the game, false if it cannot

  // Made public for testing - TODO - make it protected or sth, friend classes.
  bool isStuck();    // check if the game is "Stuck"/"Lost"
  bool isCleared();  // check if the game is "Cleared"/"Finished"
  bool isSolved();   // check if the game is "Solved"/"Won"

  Run getSourceRun(Source, unsigned int);  // returns count-sized run (or less)
  Run getDestRun(Dest);                    // returns empty or 1-card run

  std::vector<Move> getLegalMoves();
  std::vector<std::string> getLegalCommands();
};

#endif  // BOARD_H
