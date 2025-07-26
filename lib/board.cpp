/* board.cpp
 * contains some stuff to define board for solitaire play.
 * klondike specfiically, for now.
 * @kemcbride/@ke2mcbri 2020
 */

#include "board.h"

using namespace std;
using namespace solitaire;

string prettyprint_duration(chrono::milliseconds dur) {
  using namespace std::chrono;
  using days = duration<int, std::ratio<86400>>;
  auto d = duration_cast<days>(dur);
  dur -= d;
  auto h = duration_cast<hours>(dur);
  dur -= h;
  auto m = duration_cast<minutes>(dur);
  dur -= m;
  auto s = duration_cast<seconds>(dur);
  dur -= s;
  auto ms = duration_cast<milliseconds>(dur);

  auto dc = d.count();
  auto hc = h.count();
  auto mc = m.count();
  auto sc = s.count();
  auto msc = ms.count();

  std::stringstream ss;
  ss.fill('0');
  if (dc) {
    ss << d.count() << "d";
  }
  if (dc || hc) {
    if (dc) {
      ss << std::setw(2);
    }  // pad if second set of numbers
    ss << h.count() << "h";
  }
  if (dc || hc || mc) {
    if (dc || hc) {
      ss << std::setw(2);
    }
    ss << m.count() << "m";
  }
  if (dc || hc || mc || sc) {
    if (dc || hc || mc) {
      ss << std::setw(2);
    }
    ss << s.count() << 's';
  }
  if (dc || hc || mc || sc || msc) {
    if (dc || hc || mc || sc) {
      ss << std::setw(2);
    }
    ss << ms.count() << "ms";
  }

  return ss.str();
}

Board::Board(Deck &d, bool auto_solve, bool auto_reveal,
             bool recycle_penalty_enabled)
    : auto_solve(auto_solve),
      auto_reveal(auto_reveal),
      recycle_penalty_enabled(recycle_penalty_enabled),
      game_start(chrono::system_clock::now()),
      tableau(d),
      stock(d) {
  // Assuming you start with a full deck!
  // First we fill the tableau.
  // Then, using the same deck, we fill the stock.
  // Then we create the foundations.
  vector<Foundation> my_fdns;
  for (int i = 0; i < 4; ++i) {
    my_fdns.push_back(Foundation());
  }
  foundations = my_fdns;
  getAllDests();
  _setupLocPairMoveMap();
  legal_moves = allLegalMoves();
  // also initializes legal_moves, all_possible_moves, possibleMovesPerLocPair
  is_stuck = isStuck();
}

string Board::toString() {
  string board_str;

  string fdns_str;
  for (unsigned int i = 0; i < foundations.size(); ++i) {
    fdns_str += foundations.at(i).toString();
    if (i != foundations.size() - 1) {
      fdns_str += " ";
    }
  }

  string stock_str = stock.toString();
  string tbl_str = tableau.toString();

  if (show_labels == true) {
    string extra_help = " f1    f2    f3    f4      s";
    extra_help += "  |moves: " + to_string(legal_moves.size()) + "|\n";
    board_str += extra_help;
  }

  string score_str = "|Score: " + to_string(getScore()) + "|";
  board_str += fdns_str + "   " + stock_str + "  " + score_str + '\n' + tbl_str;
  return board_str;
}

void Board::enableAutoSolve() { auto_solve = true; }

void Board::disableAutoSolve() { auto_solve = false; }

void Board::enableAutoReveal() { auto_reveal = true; }

void Board::disableAutoReveal() { auto_reveal = false; }

void Board::toggleLabels() {
  show_labels = !(show_labels);
  tableau.toggleLabels();
}

void Board::next() {
  // Update hints & possible moves
  // Note: we call isStuck after updating the stock to ensure that
  // we are populating with moves for the new stock, not the previous stock
  bool recycled = stock.next();
  if (recycle_penalty_enabled && recycled)
    score = std::max(0, score + RECYCLE_STOCK_VALUE);
  // Don't need to evaluate this per next() actually, because we calculate
  // possibleMoves() using all stock values, rather than just the top.
  // is_stuck = isStuck();
  _updatePossibleMovesNext();
}

string Board::hint() {
  if (is_stuck) return "No moves are possible. Please start a new game.";

  if (hint_idx >= legal_moves.size()) {
    hint_idx = 0;
  }

  Move m = legal_moves.at(hint_idx);
  hint_idx++;
  Source s = m.getSrc();
  Card src_top = m.getSrcRun().peek().value();
  // Translate source hints for hidden sources into "next"
  if (s.type == 's' && (src_top != stock.peek().value())) return "next";
  return m.toString();
}

bool Board::foundationsFull() {
  for (auto f : foundations) {
    if (!f.isFull()) return false;
  }
  return true;
}

bool Board::isSolved() {
  if (!stock.empty()) {
    return false;
  }

  for (auto p : tableau.piles) {
    for (auto r : p.runs) {
      if (!(r.isRevealed())) {
        return false;
      }
    }
  }
  return true;
}

bool Board::isCleared() {
  bool tableauEmpty = tableau.isEmpty();
  if (!tableauEmpty) return false;
  bool fdnsFull = foundationsFull();
  if (!fdnsFull) return false;

  if (is_solved) game_end = chrono::system_clock::now();
  return is_solved;
}

void Board::reveal_top_runs() {
  int num_reveals = tableau.reveal_top_runs();
  score += num_reveals * REVEAL_VALUE;
}

void Board::solve() {
  // Don't do anything if the game isn't solved, as a backstop here.
  if (!is_solved) {
    cout << "Board is not solved; can't just finish it off from here" << endl;
    return;
  }

  while (!(is_cleared = isCleared())) {
    for (auto m : allLegalMoves()) {
      if (m.getMoveType() == TBL2FDN) {
        try {
          _move(m);
        } catch (std::runtime_error &e) {
          continue;
        }
        break;
      }
    }
  }
}

void Board::move(MoveCmd mcmd) {
  Run srcRun = getSourceRun(mcmd.getSource(), mcmd.getCount());
  Run dstRun = getDestRun(mcmd.getDest());
  Move m(srcRun, mcmd.getSource(), dstRun, mcmd.getDest(), mcmd.getCount());
  move(m);
}

void Board::move(Move m) {
  _move(m);
  _move_post_processing(m);
}

bool Board::_move(Move m) {
  Source srcLoc = m.getSrc();
  Dest dstLoc = m.getDst();
  Run srcRun = m.getSrcRun();
  Run dstRun = m.getDstRun();
  unsigned int count = m.getCount();

  if (srcLoc.type == 's') {  // source: stock
    optional<Card> stock_card = stock.peek();
    if (!stock_card.has_value())
      throw runtime_error("Invalid move: No cards remaining in stock");

    Card c = stock_card.value();

    if (dstLoc.type == 'f') {  // stock -> foundation
      Foundation &f = foundations.at(dstLoc.idx);
      if (f.canPush(c)) {
        f.push(c);
        (void)stock.pop();
        // success! update score.
        score += STK_2_FDN_VALUE;
      } else {
        throw runtime_error("Invalid move: cannot move " + c.toString() +
                            " to foundation");
      }
    } else {  // stock -> pile
      Pile &target_pile = tableau.piles.at(dstLoc.idx);
      optional<Card> c = stock.peek();
      if (c.has_value()) {
        target_pile.put(c.value());
        stock.pop();
        // success! update score.
        score += STK_2_TBL_VALUE;
      }
    }
  } else if (srcLoc.type == 'f') {  // source: foundation
    // only foundation -> pile supported
    if (dstLoc.type != 'p')
      throw runtime_error(
          "Invalid move: from foundation, only pile is valid dest.");

    unsigned int i = srcLoc.idx;
    Foundation &f = foundations.at(i);
    optional<Card> fdn_card = f.peek();
    if (!fdn_card.has_value())
      throw runtime_error("Invalid move: No cards in foundation ");
    // TODO: is there enough validation happening here?
    //   how do we know if the pile can actually receive the fdn card?
    //   (for scoring)
    Card c = fdn_card.value();
    Pile &p = tableau.piles.at(dstLoc.idx);
    p.put(c);
    (void)f.pop();
    // success! update score.
    score += FDN_2_TBL_VALUE;
  } else {                     // source: pile
    if (dstLoc.type == 'f') {  // pile -> foundation
      // move from pile to fdn
      Pile &target_pile = tableau.piles.at(srcLoc.idx);
      optional<Card> c = target_pile.peek();
      if (c.has_value()) {
        Foundation &fdn = foundations.at(dstLoc.idx);
        if (fdn.canPush(c.value())) {
          fdn.push(c.value());
          optional<Run> _ = target_pile.pop();
          // successf! update score.
          score += TBL_2_FDN_VALUE;
        } else {
          throw runtime_error("Invalid move: Cannot move " + c->toString() +
                              " to foundation");
        }
      }
    } else {  // pile -> pile
      Pile &src_pile = tableau.piles.at(srcLoc.idx);
      Pile &dst_pile = tableau.piles.at(dstLoc.idx);
      optional<Run> maybe_run = src_pile.peek(count);
      if (maybe_run.has_value()) {
        Run r = maybe_run.value();
        try {
          dst_pile.put(r);
          src_pile.take(count);
        } catch (runtime_error &e) {
          cerr << "Invalid move: doesn't satisfy run constraints" << endl;
        }
      }
    }

    if (auto_reveal) {
      reveal_top_runs();
    }
  }

  return true;
}

void Board::_move_post_processing(Move &m) {
  // Update isSolved(), isStuck(), isCleared() state - have you won?
  is_solved = isSolved();
  _updatePossibleMovesMap(m);
  is_stuck = isStuck();
  is_cleared = isCleared();

  if (auto_solve && is_solved && !is_cleared) {
    trySolve();
    game_duration =
        chrono::duration_cast<chrono::milliseconds>(game_end - game_start);
  }

  if (is_stuck) {
    cerr << "You're out of legal moves!" << endl;
    cerr << "Game time: " << prettyprint_duration(game_duration) << endl;
    cerr << "Game score: " << getScore() << endl;
    cerr << "Deal a new game using the 'restart' command" << endl;
  }
}

void Board::_setupLocPairMoveMap() {
  // Populate the keys/values
  for (auto const &s : getAllSourcesButStock()) {
    for (auto const &d : getAllDests()) {
      LocPair lp(s, d);
      set<Move> moveset;
      _possibleMovesPerLocPair.insert({lp, moveset});
    }
  }
  Source stock_source('s', 0);
  for (auto const &d : getAllDests()) {
    LocPair lp(stock_source, d);
    set<Move> moveset;
    _possibleMovesPerLocPair.insert({lp, moveset});
  }

  // Populate the starting moves
  // TODO - cleanup this copy pasta - merge it into the above loops, probably
  set<Source> all_sources = getAllSourcesButStock();

  for (auto const &s : all_sources) {
    for (auto const &d : all_dests) {
      if (s.type == 'p') {
        const Pile &p = tableau.piles.at(s.idx);
        if (!p.runs.empty()) {
          const Run &r = p.runs.back();
          if (!r.cards.empty()) {
            for (auto i : getAllCounts(r)) {
              if (i > 1 && d.type == 'f') continue;
              Run srcRun = getSourceRun(s, i);
              Run dstRun = getDestRun(d);

              if (d.type == 'p' && !dstRun.canAdd(srcRun)) continue;
              if (d.type == 'f') {
                Foundation f = foundations.at(d.idx);
                Card c = srcRun.cards.front();
                if (!f.canPush(c)) continue;
              }

              Move m(srcRun, s, dstRun, d, i);
              LocPair lp(s, d);
              _addMoveToPossibleMoves(lp, m);
            }
          }
        }
      } else if (s.type == 'f') {
        if (d.type == 'f') continue;

        Foundation &f = foundations.at(s.idx);
        if (!f.cards.empty()) {
          const Run &srcRun = f.peek().value();
          const Run &dstRun = getDestRun(d);
          Move m(srcRun, s, dstRun, d, 1);
          LocPair lp(s, d);
          _addMoveToPossibleMoves(lp, m);
        }
      }
    }
  }
  for (auto &c : stock.cards) {
    const Run &srcRun = Run(c);
    for (auto &d : all_dests) {
      const Run &dstRun = getDestRun(d);
      Move m(srcRun, stock_source, dstRun, d, 1);
      LocPair lp(stock_source, d);
      _addMoveToPossibleMoves(lp, m);
    }
  }
}

void Board::_updatePossibleMovesMap(Move &m) {
  Location src = m.getSrc();
  Location dst = m.getDst();

  auto affectedLocPairs =
      _possibleMovesPerLocPair |
      ranges::views::transform(
          [](pair<LocPair, set<Move>> lpp) { return lpp.first; }) |
      ranges::views::filter([src, dst](LocPair lp) {
        return (lp.src == src || lp.dst == src || lp.src == dst ||
                lp.dst == dst);
      });

  // Every locPair in the map's keys where the source is the source,
  // or the source is the dest, OR the dest is the source, or the dest
  // is the dest
  // Should be like around 8x4 to 12x4 total - between 32 & 48
  for (auto lp : affectedLocPairs) {
    _updatePossibleMoves(lp);
  }
}

void Board::_updatePossibleMovesNext() {
  // OK, we need to update all pairs involving the stock.
  // You can't use the stock as a destination, either, so...
  // Only loc pairs where stock is the source.
  Location stock_source('s', 0);

  auto affectedLocPairs =
      _possibleMovesPerLocPair |
      ranges::views::transform(
          [](pair<LocPair, set<Move>> lpp) { return lpp.first; }) |
      ranges::views::filter([stock_source](LocPair lp) {
        return (lp.src == stock_source || lp.dst == stock_source);
      });

  for (auto lp : affectedLocPairs) {
    _updatePossibleMoves(lp);
  }
}

void Board::_updatePossibleMoves(LocPair lp) {
  // OK - this is simple mode - for a given LocPair,
  // completely update the moves for that specific pair based
  // on current board state.
  set<Move> lpMoves;
  Location s = lp.src;
  Location d = lp.dst;

  if (s.type == 'p') {
    const Pile &p = tableau.piles.at(s.idx);
    if (!p.runs.empty()) {
      const Run &r = p.runs.back();
      if (!r.cards.empty()) {
        for (auto i : getAllCounts(r)) {
          if (i > 1 && d.type == 'f') continue;
          Run srcRun = getSourceRun(Source(s), i);
          Run dstRun = getDestRun(Dest(d));

          if (d.type == 'p' && !dstRun.canAdd(srcRun)) continue;
          if (d.type == 'f') {
            Foundation f = foundations.at(d.idx);
            Card c = srcRun.cards.front();
            if (!f.canPush(c)) continue;
          }

          Move m(srcRun, s, dstRun, d, i);
          lpMoves.insert(m);
        }
      }
    }
  } else if (s.type == 'f') {
    if (d.type == 'f') {
      _possibleMovesPerLocPair[lp] = lpMoves;
      return;
    };

    Foundation &f = foundations.at(s.idx);
    if (!f.cards.empty()) {
      const Run &srcRun = f.peek().value();
      const Run &dstRun = getDestRun(d);
      Move m(srcRun, s, dstRun, d, 1);
      lpMoves.insert(m);
    }
  } else if (s.type == 's') {
    for (auto &c : stock.cards) {
      const Run &srcRun = Run(c);
      for (auto &d : all_dests) {
        const Run &dstRun = getDestRun(d);
        Source stock_source('s', 0);
        Move m(srcRun, stock_source, dstRun, d, 1);
        lpMoves.insert(m);
      }
    }
  }

  _possibleMovesPerLocPair[lp] = lpMoves;
}

void Board::_addMoveToPossibleMoves(LocPair lp, Move m) {
  set<Move> &moveset = _possibleMovesPerLocPair[lp];
  moveset.insert(m);
}

set<Source> Board::getAllSourcesButStock() {
  set<Source> all_sources;
  for (unsigned int i = 0; i < foundations.size(); ++i) {
    // It's only a valid source if there are some cards there.
    Foundation f = foundations.at(i);
    if (!f.cards.empty()) all_sources.insert(Source('f', i));
  }
  for (unsigned int i = 0; i < tableau.piles.size(); ++i) {
    // It's only a valid source if there are some cards there.
    Pile p = tableau.piles.at(i);
    if (!p.runs.empty()) all_sources.insert(Source('p', i));
  }
  return all_sources;
}

set<Dest> Board::getAllDests() {
  if (!all_dests.empty()) return all_dests;

  for (unsigned int i = 0; i < foundations.size(); ++i) {
    all_dests.insert(Dest('f', i));
  }
  for (unsigned int i = 0; i < tableau.piles.size(); ++i) {
    all_dests.insert(Dest('p', i));
  }
  return all_dests;
}

vector<int> Board::getAllCounts(Run r) {
  // return list of possible move sizes from this run
  std::vector<int> v(r.cards.size());
  std::iota(v.begin(), v.end(), 1);
  return v;
}

int Board::getScore() { return score; }

bool Board::isLegal(Move m) const {
  Dest d = m.getDst();
  Run dstRun = m.getDstRun();
  Run srcRun = m.getSrcRun();

  if (m.getSrc().type == 'f' && d.type == 'f') return false;

  if (d.type == 'p') {
    return dstRun.canAdd(srcRun);
  } else if (d.type == 'f') {
    if (srcRun.cards.size() != 1) return false;
    Foundation f = foundations.at(d.idx);
    Card c = srcRun.cards.front();
    return f.canPush(c);
  }
  return false;
}

bool Board::isMeaningful(Move m) const {
  // moves of a pile with 1 run starting with K to an empty pile are NOT
  // meaningful
  if (m.getSrc().type == 'p' && m.getDst().type == 'p') {
    const Pile &srcPile = tableau.piles.at(m.getSrc().idx);
    const Pile &dstPile = tableau.piles.at(m.getDst().idx);
    if (srcPile.runs.size() == 1 && dstPile.runs.size() == 0) {
      auto srcRun = srcPile.runs.front();  // there's only one /shrug
      auto backCard = srcRun.cards.front();
      if (backCard.getRank().getValue() == 13) return false;
    }
  }
  return true;
}

vector<Move> Board::allLegalMoves() {
  set<Move> all_legal_moves;

  for (auto p : _possibleMovesPerLocPair) {
    all_legal_moves.merge(p.second);
  }
  auto blah = all_legal_moves | std::ranges::views::filter([*this](Move m) {
                return this->isLegal(m) && this->isMeaningful(m);
              });
  vector<Move> bloo(blah.begin(), blah.end());
  return bloo;
}

vector<string> Board::allLegalCommands() {
  set<string> cmds;
  for (auto m : legal_moves) {
    // Moves from the stock should be translated to 'next' if curr stock !=
    // that source
    Source s = m.getSrc();
    Card src_top = m.getSrcRun().peek().value();
    if (s.type == 's' && (src_top != stock.peek().value())) {
      cmds.insert("next");
    } else {
      cmds.insert(m.toString());
    }
  }
  return vector<string>(cmds.begin(), cmds.end());
}

bool Board::isStuck() {
  legal_moves = allLegalMoves();
  legal_commands = allLegalCommands();
  if (legal_commands.size() == 0) {
    return true;
  }
  return false;
}

bool Board::trySolve() {
  if (is_solved) {
    solve();
    // _move_post_processing();
    is_solved = isSolved();
    is_stuck = isStuck();
    is_cleared = isCleared();
    cerr << "Game has been won! Good job, good job." << endl;
    cerr << "Game time: " << prettyprint_duration(game_duration) << endl;
    cerr << "Game score: " << getScore() << endl;
    cerr << "Deal a new game using the 'restart' command" << endl;
    return true;
  }
  return false;
}

Run Board::getSourceRun(Source s, unsigned int count) {
  Run srcRun;
  if (s.type == 's') {  // source: stock
    optional<Card> stock_card = stock.peek();
    if (!stock_card.has_value())
      throw runtime_error(
          "B:getSourceRun:Invalid move: No cards remaining in stock");
    Card c = stock_card.value();
    srcRun = Run(c);

  } else if (s.type == 'f') {  // source: foundation
    unsigned int i = s.idx;
    Foundation &f = foundations.at(i);
    optional<Card> fdn_card = f.peek();
    if (!fdn_card.has_value())
      throw runtime_error(
          "B:getSourceRun:Invalid move: No cards in foundation ");
    Card c = fdn_card.value();
    srcRun = Run(c);

  } else {  // source: pile
    Pile &src_pile = tableau.piles.at(s.idx);
    optional<Run> opt_src_run = src_pile.peek(count);
    if (opt_src_run.has_value())
      srcRun = Run(opt_src_run.value());
    else
      throw runtime_error(
          "B:getSourceRun:Invalid move: Tried to take from empty pile");
  }
  return srcRun;
}

Run Board::getDestRun(Dest d) {
  Run dstRun;  // default is empty run
  // Now set up the dest run for this move (expected: 1 or 0 cards)
  if (d.type == 'f') {
    Foundation f = foundations.at(d.idx);
    optional<Card> c = f.peek();
    if (c.has_value()) dstRun = Run(c.value());

  } else if (d.type == 'p') {
    Pile p = tableau.piles.at(d.idx);
    optional<Card> c = p.peek();
    if (c.has_value()) dstRun = Run(c.value());

  } else
    throw runtime_error("B:getDestRun: stock is not a valid destination");
  return dstRun;
}

std::vector<Move> Board::getLegalMoves() { return legal_moves; }

std::vector<std::string> Board::getLegalCommands() { return legal_commands; }
