/* board.cpp
 * contains some stuff to define board for solitaire play.
 * klondike specfiically, for now.
 * @kemcbride/@ke2mcbri 2020
 */

#include "board.h"

using namespace std;

Board::Board(Deck &d) : tableau(d), stock(d) {
  // Assuming you start with a full deck!
  // First we fill the tableau.
  // Then, using the same deck, we fill the stock.
  // Then we create the foundations.
  vector<Foundation> my_fdns;
  for (int i = 0; i < 4; ++i) {
    my_fdns.push_back(Foundation());
  }
  this->foundations = my_fdns;
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
    string extra_help = " f1    f2    f3    f4      s\n";
    board_str += extra_help;
  }

  board_str += fdns_str + "   " + stock_str + '\n' + tbl_str;
  return board_str;
}

void Board::toggle_labels() {
  this->show_labels = !(this->show_labels);
  this->tableau.toggle_labels();
}

void Board::next() { this->stock.next(); }

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
  return true;
}

void Board::reveal_top_runs() { this->tableau.reveal_top_runs(); }

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
      }
    } else { // stock -> pile
      Pile &target_pile = this->tableau.piles.at(dstLoc.idx);
      optional<Card> c = this->stock.peek();
      if (c.has_value()) {
        target_pile.put(c.value());
        this->stock.pop();
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

    Card c = fdn_card.value();
    Pile &p = this->tableau.piles.at(dstLoc.idx);
    p.put(c);
    (void)f.pop();
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
    this->reveal_top_runs();
  }

  // Update isSolved() state - have you won?
  this->is_solved = this->isSolved();
  this->is_stuck = this->isStuck();
  if (this->is_solved) {
    cerr << "Game has been won! Good job, good job." << endl;
    cerr << "Deal a new game using the 'restart' command" << endl;
  } else if (this->is_stuck) {
    cerr << "You're out of legal moves!" << endl;
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
    }
    for (auto c : this->stock.cards) {
      Run srcRun = Run(c);
      for (auto d : all_dests) {
        Run dstRun = this->getDestRun(d);
        moves.push_back(Move(srcRun, stock_source, dstRun, d, 1));
      }
    }
  }
  return moves;
}

bool Board::isLegal(Move m) {
  Dest d = m.getDst();
  Run dstRun = m.getDstRun();
  Run srcRun = m.getSrcRun();

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

vector<Move> Board::allLegalMoves() {
  vector<Move> all_legal_moves;
  vector<Move> all_possible_moves = this->allPossibleMoves();
  for (auto m : all_possible_moves) {
    if (this->isLegal(m)) {
      all_legal_moves.push_back(m);
    }
  }
  return all_legal_moves;
}

bool Board::isStuck() {
  vector<Move> legal_moves = this->allLegalMoves();
  if (legal_moves.size() == 0) {
    return true;
  }
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