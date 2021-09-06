/* board.cpp
 * contains some stuff to define board for solitaire play.
 * klondike specfiically, for now.
 * @kemcbride/@ke2mcbri 2020
 */

#include "board.h"

using namespace std;

string prettyprint_duration(chrono::seconds dur) {
  using namespace std::chrono;
  using days = duration<int, std::ratio<86400>>;
  auto d = duration_cast<days>(dur);
  dur -= d;
  auto h = duration_cast<hours>(dur);
  dur -= h;
  auto m = duration_cast<minutes>(dur);
  dur -= m;
  auto s = duration_cast<seconds>(dur);

  auto dc = d.count();
  auto hc = h.count();
  auto mc = m.count();
  auto sc = s.count();

  std::stringstream ss;
  ss.fill('0');
  if (dc) {
      ss << d.count() << "d";
  }
  if (dc || hc) {
      if (dc) { ss << std::setw(2); } //pad if second set of numbers
      ss << h.count() << "h";
  }
  if (dc || hc || mc) {
      if (dc || hc) { ss << std::setw(2); }
      ss << m.count() << "m";
  }
  if (dc || hc || mc || sc) {
      if (dc || hc || mc) { ss << std::setw(2); }
      ss << s.count() << 's';
  }

  return ss.str();
}

Board::Board(Deck &d) : game_start(chrono::system_clock::now()), tableau(d), stock(d) {
  // Assuming you start with a full deck!
  // First we fill the tableau.
  // Then, using the same deck, we fill the stock.
  // Then we create the foundations.
  vector<Foundation> my_fdns;
  for (int i = 0; i < 4; ++i) {
    my_fdns.push_back(Foundation());
  }
  this->foundations = my_fdns;
  this->is_stuck = this->isStuck(); // also initializes this->legal_moves
}

string Board::toString() {
  string board_str;

  string fdns_str;
  for (unsigned int i = 0; i < this->foundations.size(); ++i) {
    fdns_str += this->foundations.at(i).toString();
    if (i != this->foundations.size() - 1) {
      fdns_str += " ";
    }
  }

  string stock_str = this->stock.toString();
  string tbl_str = this->tableau.toString();

  if (this->show_labels == true) {
    string extra_help = " f1    f2    f3    f4      s";
    extra_help += "  |moves: " + to_string(this->legal_commands.size()) + "|\n";
    board_str += extra_help;
  }

  string score_str = "|Score: " + to_string(this->getScore()) + "|";
  board_str += fdns_str + "   " + stock_str + "  " + score_str + '\n' + tbl_str;
  return board_str;
}

void Board::enableAutoSolve() {
  this->auto_solve = true;
}

void Board::disableAutoSolve() {
  this->auto_solve = false;
}

void Board::enableAutoReveal() {
  this->auto_reveal = true;
}

void Board::disableAutoReveal() {
  this->auto_reveal = false;
}

void Board::toggleLabels() {
  this->show_labels = !(this->show_labels);
  this->tableau.toggleLabels();
}

void Board::next() {
  // Update hints & possible moves
  // Note: we call isStuck after updating the stock to ensure that
  // we are populating with moves for the new stock, not the previous stock
  this->stock.next();
  this->is_stuck = this->isStuck();
}

string Board::hint() {
  if (this->is_stuck)
    return "No moves are possible. Please start a new game.";

  if (this->hint_idx >= this->legal_commands.size())
    this->hint_idx = 0;

  string cmd = this->legal_commands.at(this->hint_idx);
  this->hint_idx++;
  return cmd;
}

bool Board::foundationsFull() {
  for (auto f : this->foundations) {
    if (!f.isFull()) return false;
  }
  return true;
}

bool Board::isSolved() {
  if (!(this->stock.cards.size() == 0)) {
    return false;
  }

  for (auto p : this->tableau.piles) {
    for (auto r : p.runs) {
      if (!(r.isRevealed())) {
        return false;
      }
    }
  }
  this->game_end = chrono::system_clock::now();
  return true;
}

bool Board::isCleared() {
  bool tableauEmpty = this->tableau.isEmpty();
  if (!tableauEmpty) return false;
  bool fdnsFull = this->foundationsFull();
  if (!fdnsFull) return false;
  return this->is_solved;
}

void Board::reveal_top_runs() {
  int num_reveals = this->tableau.reveal_top_runs();
  this->score += num_reveals * REVEAL_VALUE;
}

void Board::solve() {
  // Don't do anything if the game isn't solved, as a backstop here.
  if (!this->is_solved) {
    cout << "Board is not solved; can't just finish it off from here" << endl;
    return;
  }

  cout << "In board::solve, starting solve loop..." << endl;
  // TODO: somehow, this stalls...
  while (!this->isCleared()) {
    for (auto m : this->allLegalMoves()) {
      cout << "Examining possible move: " << m.toString() << endl;
      if (m.getMoveType() == TBL2FDN) {
        cout << "executing move: " << m.toString() << endl;
        this->move(m);
        cout << this->toString() << endl;
        break;
      }
    }
    cout << "exited for move in all legal moves loop" << endl;
  }
  cout << "exited while this is not cleared loop" << endl;
}

void Board::move(MoveCmd mcmd) {
  Run srcRun = this->getSourceRun(mcmd.getSource(), mcmd.getCount());
  Run dstRun = this->getDestRun(mcmd.getDest());
  Move m(srcRun, mcmd.getSource(), dstRun, mcmd.getDest(), mcmd.getCount());
  this->move(m);
}

void Board::move(Move m) {
  Source srcLoc = m.getSrc();
  Dest dstLoc = m.getDst();
  Run srcRun = m.getSrcRun();
  Run dstRun = m.getDstRun();
  unsigned int count = m.getCount();

  if (srcLoc.type == 's') { // source: stock
    optional<Card> stock_card = this->stock.peek();
    if (!stock_card.has_value())
      throw runtime_error("Invalid move: No cards remaining in stock");

    Card c = stock_card.value();

    if (dstLoc.type == 'f') { // stock -> foundation
      Foundation &f = this->foundations.at(dstLoc.idx);
      if (f.canPush(c)) {
        f.push(c);
        (void)this->stock.pop();
        // success! update score.
        this->score += STK_2_FDN_VALUE;
      }
    } else { // stock -> pile
      Pile &target_pile = this->tableau.piles.at(dstLoc.idx);
      optional<Card> c = this->stock.peek();
      if (c.has_value()) {
        target_pile.put(c.value());
        this->stock.pop();
        // success! update score.
        this->score += STK_2_TBL_VALUE;
      }
    }
  } else if (srcLoc.type == 'f') { // source: foundation
    // only foundation -> pile supported
    if (dstLoc.type != 'p')
      throw runtime_error(
          "Invalid move: from foundation, only pile is valid dest.");

    unsigned int i = srcLoc.idx;
    Foundation &f = this->foundations.at(i);
    optional<Card> fdn_card = f.peek();
    if (!fdn_card.has_value())
      throw runtime_error("Invalid move: No cards in foundation ");
    // TODO: is there enough validation happening here?
    //   how do we know if the pile can actually receive the fdn card?
    //   (for scoring)
    Card c = fdn_card.value();
    Pile &p = this->tableau.piles.at(dstLoc.idx);
    p.put(c);
    (void)f.pop();
    // success! update score.
    this->score += FDN_2_TBL_VALUE;
  } else {                    // source: pile
    if (dstLoc.type == 'f') { // pile -> foundation
      // move from pile to fdn
      Pile &target_pile = this->tableau.piles.at(srcLoc.idx);
      optional<Card> c = target_pile.peek();
      if (c.has_value()) {
        Foundation &fdn = this->foundations.at(dstLoc.idx);
        if (fdn.canPush(c.value())) {
          fdn.push(c.value());
          optional<Run> _ = target_pile.pop();
          // successf! update score.
          this->score += TBL_2_FDN_VALUE;
        }
      }
    } else { // pile -> pile
      Pile &src_pile = this->tableau.piles.at(srcLoc.idx);
      Pile &dst_pile = this->tableau.piles.at(dstLoc.idx);
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
    
    if (this->auto_reveal) {
      this->reveal_top_runs();
    }
  }

  // Update isSolved(), isStuck(), isCleared() state - have you won?
  this->is_solved = this->isSolved();
  this->is_stuck = this->isStuck();
  this->is_cleared = this->isCleared();
  auto game_duration = chrono::duration_cast<chrono::seconds>(this->game_end - this->game_start);
  if (this->is_cleared) {
    cerr << "Game has been won! Good job, good job." << endl;
    cerr << "Game time: " << prettyprint_duration(game_duration) << endl;
    cerr << "Game score: " << this->getScore() << endl;
    cerr << "Deal a new game using the 'restart' command" << endl;
  } else if (this->is_stuck) {
    cerr << "You're out of legal moves!" << endl;
    cerr << "Game time: " << prettyprint_duration(game_duration) << endl;
    cerr << "Game score: " << this->getScore() << endl;
    cerr << "Deal a new game using the 'restart' command" << endl;
  }
}

vector<Source> Board::getAllSourcesButStock() {
  vector<Source> all_sources;
  for (unsigned int i = 0; i < this->foundations.size(); ++i) {
    // It's only a valid source if there are some cards there.
    Foundation f = this->foundations.at(i);
    if (!f.cards.empty())
      all_sources.push_back(Source('f', i));
  }
  for (unsigned int i = 0; i < this->tableau.piles.size(); ++i) {
    // It's only a valid source if there are some cards there.
    Pile p = this->tableau.piles.at(i);
    if (!p.runs.empty())
      all_sources.push_back(Source('p', i));
  }
  return all_sources;
}

vector<Dest> Board::getAllDests() {
  vector<Dest> all_dests;
  for (unsigned int i = 0; i < this->foundations.size(); ++i) {
    all_dests.push_back(Dest('f', i));
  }
  for (unsigned int i = 0; i < this->tableau.piles.size(); ++i) {
    all_dests.push_back(Dest('p', i));
  }
  return all_dests;
}

vector<int> Board::getAllCounts(Run r) {
  return vector<int>(1, r.cards.size());
}

vector<Move> Board::allPossibleMoves() {
  vector<Move> moves;

  Source stock_source('s', 0);
  vector<Source> all_sources = this->getAllSourcesButStock();
  vector<Dest> all_dests = this->getAllDests();

  for (auto s : all_sources) {
    for (auto d : all_dests) {
      // In order to figure out how many different moves we can do from this
      // source, to this dest, we need to know how many cards there are in the
      // run
      if (s.type == 'p') {
        Pile p = this->tableau.piles.at(s.idx);
        if (!p.runs.empty()) {
          Run r = p.runs.back();
          if (!r.cards.empty()) {
            for (auto i : this->getAllCounts(r)) {
              Run srcRun = this->getSourceRun(s, i);
              Run dstRun = this->getDestRun(d);
              moves.push_back(Move(srcRun, s, dstRun, d, i));
            }
          }
        }
      } else if (s.type == 'f') {
        Foundation f = this->foundations.at(s.idx);
        if (!f.cards.empty()) {
          Run srcRun = f.peek().value();
          Run dstRun = this->getDestRun(d);
          moves.push_back(Move(srcRun, s, dstRun, d, 1));
        }
      }
    }
  }
  for (auto c : this->stock.cards) {
    Run srcRun = Run(c);
    for (auto d : all_dests) {
      Run dstRun = this->getDestRun(d);
      moves.push_back(Move(srcRun, stock_source, dstRun, d, 1));
    }
  }
  return moves;
}

int Board::getScore() {
  return this->score;
}

bool Board::isLegal(Move m) {
  Dest d = m.getDst();
  Run dstRun = m.getDstRun();
  Run srcRun = m.getSrcRun();

  // moves from one foundation to another are NOT legal
  if (m.getSrc().type == 'f' && m.getDst().type == 'f')
    return false;

  if (d.type == 'p') {
    return dstRun.canAdd(srcRun);
  } else if (d.type == 'f') {
    if (srcRun.cards.size() != 1)
      return false;
    Foundation f = this->foundations.at(d.idx);
    Card c = srcRun.cards.front();
    return f.canPush(c);
  }
  return false;
}

bool Board::isMeaningful(Move m) {
  // moves of a pile with 1 run starting with K to an empty pile are NOT meaningful
  if (m.getSrc().type == 'p' && m.getDst().type == 'p') {
    Pile &srcPile = this->tableau.piles.at(m.getSrc().idx);
    Pile &dstPile = this->tableau.piles.at(m.getDst().idx);
    if (srcPile.runs.size() == 1 && dstPile.runs.size() == 0) {
      auto srcRun = srcPile.runs.front(); // there's only one /shrug
      auto backCard = srcRun.cards.front();
      if (backCard.getRank().getValue() == 13)
        return false;
    }
  }
  return true;
}

vector<Move> Board::allLegalMoves() {
  set<Move> all_legal_moves;
  vector<Move> all_possible_moves = this->allPossibleMoves();
  for (auto m : all_possible_moves) {
    if (this->isLegal(m) && this->isMeaningful(m))
      all_legal_moves.insert(m);
  }
  vector<Move> legal_moves(all_legal_moves.begin(), all_legal_moves.end());
  return legal_moves;
}

vector<string> Board::allLegalCommands() {
  set<string> cmds;
  for (auto m : this->legal_moves) {
    // Moves from the stock should be translated to 'next' if curr stock != that source
    Source s = m.getSrc();
    Card src_top = m.getSrcRun().peek().value();
    if ( s.type == 's' && (src_top != this->stock.peek().value())) {
      cmds.insert("next");
    } else {
      cmds.insert(m.toString());
    }
  } // for
  vector<string> legal_cmds(cmds.begin(), cmds.end());
  return legal_cmds;
}

bool Board::isStuck() {
  this->legal_moves = this->allLegalMoves();
  this->legal_commands = this->allLegalCommands();
  if (this->legal_commands.size() == 0) {
    return true;
  }
  return false;
}

bool Board::trySolve() {
  if (this->is_solved) {
    cout << "Board is solvable in current state; solving..." << endl;
    this->solve();
    return true;
  }
  cout << "Board is not solvable in current state" << endl;
  return false;
}

Run Board::getSourceRun(Source s, unsigned int count) {
  Run srcRun;
  if (s.type == 's') { // source: stock
    optional<Card> stock_card = this->stock.peek();
    if (!stock_card.has_value())
      throw runtime_error(
          "B:getSourceRun:Invalid move: No cards remaining in stock");
    Card c = stock_card.value();
    srcRun = Run(c);

  } else if (s.type == 'f') { // source: foundation
    unsigned int i = s.idx;
    Foundation &f = this->foundations.at(i);
    optional<Card> fdn_card = f.peek();
    if (!fdn_card.has_value())
      throw runtime_error(
          "B:getSourceRun:Invalid move: No cards in foundation ");
    Card c = fdn_card.value();
    srcRun = Run(c);

  } else { // source: pile
    Pile &src_pile = this->tableau.piles.at(s.idx);
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
  Run dstRun; // default is empty run
  // Now set up the dest run for this move (expected: 1 or 0 cards)
  if (d.type == 'f') {
    Foundation f = this->foundations.at(d.idx);
    optional<Card> c = f.peek();
    if (c.has_value())
      dstRun = Run(c.value());

  } else if (d.type == 'p') {
    Pile p = this->tableau.piles.at(d.idx);
    optional<Card> c = p.peek();
    if (c.has_value())
      dstRun = Run(c.value());

  } else
    throw runtime_error("B:getDestRun: stock is not a valid destination");
  return dstRun;
}
