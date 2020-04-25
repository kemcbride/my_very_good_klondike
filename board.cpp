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

void Board::move(Move m) {
  if (m.getSource().type == 's') {
    optional<Card> stock_card = this->stock.peek();
    if (!stock_card.has_value()) {
      cerr << "Invalid move: No cards remaining in stock" << endl;
      return;
    }
    Card c = stock_card.value();

    if (m.getDest().type == 'f') {
      // move from source to foundation.
      Foundation &f = this->foundations.at(m.getDest().idx-1);
      try {
        f.push(c);
        Card c = this->stock.pop().value();
      } catch (runtime_error &e) {
        cerr << "Invalid move: " << e.what() << endl;
        return;
      }
    } else {
      // source -> pile
      cerr << "source -> pile not implementd" << endl;
      return;
    }
  } else {
    cerr << "moves from pile, fdn not supported yet" << endl;
    return;
  }

  // Update isSolved() state
  this->is_solved = this->isSolved();
  if (this->is_solved) {
    cerr << "Game has been won! Good job, good job." << endl;
  }
}
