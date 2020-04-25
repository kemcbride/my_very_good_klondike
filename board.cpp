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
