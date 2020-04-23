/* board.cpp 
 * contains some stuff to define board for solitaire play.
 * klondike specfiically, for now.
 * @kemcbride/@ke2mcbri 2020
 */

#include "board.h"

using namespace std;

Board::Board(Deck d) : tableau(d), stock(d) {
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
