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
  for (int i =0; i < 4; ++i) {
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

void Board::next() {
  this->stock.next();
}

bool Board::isSolved() {
  if (!(this->stock.cards.size() == 0)) {
    return false;
  }

  for (auto p : this->tableau.piles) {
    for (auto r : p.runs) {
      if ( !(r.isRevealed()) ) {
        return false;
      }
    }
  }
  return true;
}

void Board::reveal_top_runs() {
  this->tableau.reveal_top_runs();
}

void Board::move(MoveCmd m) {
  if (m.getSource().type == 's') { // source: stock
    optional<Card> stock_card = this->stock.peek();
    if (!stock_card.has_value()) {
      cerr << "Invalid move: No cards remaining in stock" << endl;
      return;
    }
    Card c = stock_card.value();

    if (m.getDest().type == 'f') { // stock -> foundation
      Foundation &f = this->foundations.at(m.getDest().idx);
      f.push(c);
      (void) this->stock.pop();
    } else { // stock -> pile
      Pile &target_pile = this->tableau.piles.at(m.getDest().idx);
      optional<Card> c = this->stock.peek();
      if (c.has_value()) {
        target_pile.put(c.value());
        this->stock.pop();
      }
    }
  } else if (m.getSource().type == 'f') { // source: foundation
    // only foundation -> pile supported
    if (m.getDest().type != 'p')
      throw runtime_error("Invalid move: from foundation, only pile is valid dest.");

    unsigned int i = m.getSource().idx;
    Foundation &f = this->foundations.at(i);
    optional<Card> fdn_card = f.peek();
    if (!fdn_card.has_value()) {
      cerr << "Invalid move: No cards in foundation " << i << endl;
      return;
    }
    Card c = fdn_card.value();
    Pile &p = this->tableau.piles.at(m.getDest().idx);
    p.put(c);
    (void) f.pop();
  } else { // source: pile
    if (m.getDest().type == 'f') { // pile -> foundation
      // move from pile to fdn
      Pile &target_pile = this->tableau.piles.at(m.getSource().idx);
      optional<Card> c = target_pile.peek();
      if (c.has_value()) {
        Foundation &fdn = this->foundations.at(m.getDest().idx);
        fdn.push(c.value());
        optional<Run> _ = target_pile.pop();
      }
    } else { // pile -> pile
      Pile &src_pile = this->tableau.piles.at(m.getSource().idx);
      Pile &dst_pile = this->tableau.piles.at(m.getDest().idx);
      optional<Run> maybe_run = src_pile.take(m.getCount());
      if (maybe_run.has_value()) {
        Run r = maybe_run.value();
        try {
          dst_pile.put(r);
        } catch (runtime_error &e) {
          cerr << "doesn't satisfy run constraints" << endl;
          src_pile.put(r);
        }
        // TODO: need to properly handle case where this put fails
        // https://github.com/kemcbride/my_very_good_klondike/issues/2
      }
    }
    this->reveal_top_runs();
  }

  // Update isSolved() state - have you won?
  this->is_solved = this->isSolved();
  if (this->is_solved) {
    cerr << "Game has been won! Good job, good job." << endl;
    cerr << "Deal a new game using the 'restart' command" << endl;
  }
  // Causing out_of_range errors :(
  /* this->is_stuck = this->isStuck(); */
  if (this->is_stuck) {
    cerr << "You're out of legal moves!" << endl;
    cerr << "Deal a new game using the 'restart' command" << endl;
  }
}

vector<Source> Board::getAllSources() {
  vector<Source> all_sources;
  // BUG: this means we'll only check the current top of "stock"
  // But technically, we need to check every card in the stock.
  all_sources.push_back(Source('s', 0));
  for (unsigned int i = 0; i < this->foundations.size(); ++i) {
    // It's only a valid source if there are some cards there.
    Foundation f = this->foundations.at(i);
    if ( !f.cards.empty() )
      all_sources.push_back(Source('f', i));
    cerr << "getAllSources fdn iteration" << endl;
  }
  for (unsigned int i = 0; i < this->tableau.piles.size(); ++i) {
    // It's only a valid source if there are some cards there.
    cerr << "getAllSources pile iteration" << endl;
    Pile p = this->tableau.piles.at(i);
    if ( !p.runs.empty() )
      all_sources.push_back(Source('p', i));
  }
  return all_sources;
}

vector<Dest> Board::getAllDests() {
  cerr << "start of getAllDests" << endl;
  vector<Dest> all_dests;
  for (unsigned int i = 0; i < this->foundations.size(); ++i) {
    all_dests.push_back(Dest('f', i));
  }
  for (unsigned int i = 0; i < this->tableau.piles.size(); ++i) {
    all_dests.push_back(Dest('p', i));
  }
  return all_dests;
  cerr << "end of getAllDests" << endl;
}

vector<int> Board::getAllCounts(Run r) {
  return vector<int>(1, r.cards.size());
}

vector<MoveCmd> Board::allPossibleMoves() {
  vector<MoveCmd> moves; 

  vector<Source> all_sources = this->getAllSources();
  vector<Dest> all_dests = this->getAllDests();

  for ( auto s : all_sources ) {
    for ( auto d : all_dests ) {
      // In order to figure out how many different moves we can do from this 
      // source, to this dest, we need to know how many cards there are in the run
      Run r = this->tableau.piles.at(s.idx).runs.back();
      for ( auto i : this->getAllCounts(r) ) {
        moves.push_back(MoveCmd(s, d, i));
      }
    }
    // TODO
    // Logic for iterating through if any stock moves are viable should
    // probably go here.
  }
  cerr << "Hello end of all PossibleMoves" << endl;
  return moves;
}

bool Board::isLegal(MoveCmd m) {
  cerr << "start of isLegal" << endl;
  // OK - so we know that the sources are valid, so now we need to try to see
  // if the destinations will accept those moves

  // Deal with finding out if the source is valid
  // and find the "bottom" of its stack, for considering the dest later
  cerr << "checking move: " << m.getSource().type << " " << m.getSource().idx+1  << endl;
  cerr << "               " << m.getDest().type << " " << m.getDest().idx+1 << endl;
  Card source_bottom;
  if (m.getSource().type == 's') {
    optional<Card> stock_src_opt = this->stock.peek();
    if (stock_src_opt.has_value()) {
      source_bottom = stock_src_opt.value();
    } else {
      return false;
    }
  } else if (m.getSource().type == 'f') {
    Foundation& f = this->foundations.at(m.getSource().idx);
    optional<Card> fdn_src_opt = f.peek();
    if (fdn_src_opt.has_value()) {
      source_bottom = fdn_src_opt.value();
    } else {
      return false;
    }
  } else if (m.getSource().type == 'p') {
    Pile& p = this->tableau.piles.at(m.getSource().idx);
    optional<Run> run_opt = p.take(m.getCount());
    if (run_opt.has_value()) {
      Run r = run_opt.value();
      if (r.cards.size() != (unsigned int) m.getCount()) {
        return false;
      }
      source_bottom = r.cards.back();
    } else {
      return false;
    }
  }

  // Destination logic & filtering
  if (m.getDest().type == 'f') {
    if (m.getCount() > 1) return false;
    Foundation& f = this->foundations.at(m.getDest().idx);
    return f.canPush(source_bottom);
  } else if (m.getDest().type == 'p') {
    Pile& p = this->tableau.piles.at(m.getDest().idx);
    if (p.runs.empty() && source_bottom.getRank() != 'K') {
      return false;
    }
    Run& r = p.runs.back();
    return r.canAdd(source_bottom);
  }
  cerr << "Hello end of isLegal" << endl;
  return false;
}

vector<MoveCmd> Board::allLegalMoves() {
  vector<MoveCmd> all_legal_moves;
  vector<MoveCmd> all_possible_moves = this->allPossibleMoves();
  for ( auto m : all_possible_moves ) {
    if ( this->isLegal(m) ) {
      cerr << "OK: Legal move!" << endl;
      all_legal_moves.push_back(m);
    }
  }
  return all_legal_moves;
}

bool Board::isStuck() {
  vector<MoveCmd> legal_moves = this->allLegalMoves();
  if (legal_moves.size() == 0) {
    return true;
  }
  return false;
}
