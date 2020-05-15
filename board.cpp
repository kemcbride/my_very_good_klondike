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

vector<Move> Board::allPossibleMoves() {
  vector<Move> moves; 
  // In order to do this, we need to:
  // * iterate over all sources
  //   * iterate for each move size
  //     * iterate for each dest

  // Good thing I implemented Source & Dest as objects...
  // Except!! I fucked up implementing their constructors.
  // OK - what are possible move sources?
  // Foundation
  // Stock
  // Piles
  Stock& s = this->stock;
  for (auto f : this->foundations) {
    cout << "hello f!" << endl;
  }

}

void Board::reveal_top_runs() {
  this->tableau.reveal_top_runs();
}

void Board::move(Move m) {
  if (m.getSource().type == 's') { // source: stock
    optional<Card> stock_card = this->stock.peek();
    if (!stock_card.has_value()) {
      cerr << "Invalid move: No cards remaining in stock" << endl;
      return;
    }
    Card c = stock_card.value();

    if (m.getDest().type == 'f') { // stock -> foundation
      Foundation &f = this->foundations.at(m.getDest().idx-1);
      f.push(c);
      (void) this->stock.pop();
    } else { // stock -> pile
      Pile &target_pile = this->tableau.piles.at(m.getDest().idx-1);
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
    Foundation &f = this->foundations.at(i-1);
    optional<Card> fdn_card = f.peek();
    if (!fdn_card.has_value()) {
      cerr << "Invalid move: No cards in foundation " << i << endl;
      return;
    }
    Card c = fdn_card.value();
    Pile &p = this->tableau.piles.at(m.getDest().idx-1);
    p.put(c);
    (void) f.pop();
  } else { // source: pile
    if (m.getDest().type == 'f') { // pile -> foundation
      // move from pile to fdn
      Pile &target_pile = this->tableau.piles.at(m.getSource().idx-1);
      optional<Card> c = target_pile.peek();
      if (c.has_value()) {
        Foundation &fdn = this->foundations.at(m.getDest().idx-1);
        fdn.push(c.value());
        optional<Run> _ = target_pile.pop();
      }
    } else { // pile -> pile
      Pile &src_pile = this->tableau.piles.at(m.getSource().idx-1);
      Pile &dst_pile = this->tableau.piles.at(m.getDest().idx-1);
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
}
