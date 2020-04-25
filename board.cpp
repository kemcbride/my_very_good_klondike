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
  /* Tableau t(d); */
  /* // Then, using the same deck, we fill the stock. */
  /* Stock s(d); */
  // Then we create the foundations.
  vector<Foundation> my_fdns;
  for (int i =0; i < 4; ++i) {
    my_fdns.push_back(Foundation());
  }
  this->foundations = my_fdns;
}

/* Board::Board(Tableau t, Stock s) { */
/*   this->tableau = t; */
/*   this->stock = s; */

/*   vector<Foundation> my_fdns; */
/*   for (int i =0; i < 4; ++i) { */
/*     my_fdns.push_back(Foundation()); */
/*   } */
/*   this->foundations = my_fdns; */
/* } */

string Board::toString() {
  string fdns_str;
  for (unsigned int i = 0; i < this->foundations.size(); ++i) {
    fdns_str += this->foundations.at(i).toString();
    if (i != this->foundations.size() - 1) {
      fdns_str += " ";
    }
  }

  string stock_str = this->stock.toString();

  string tbl_str = this->tableau.toString();

  string board_str;
  board_str = fdns_str + '\n' + stock_str + '\n' + tbl_str;
  return board_str;
}
