/* board.cpp 
 * contains some stuff to define board for solitaire play.
 * klondike specfiically, for now.
 * @kemcbride/@ke2mcbri 2020
 */

#include "board.h"

using namespace std;


Pile::Pile(vector<Card> deal) {
  this->cards = deal;
}

// Idea: pile should be contained of "Runs" which can be
// visible or invisible, and are continguous  movablelilggcrrkrhvtnkbglfjkgvvghtrjllenhuiv
// chunks
// AKA - the below implementation sucks and I should not use it

/* Card Pile::peek() { */
/*   if (this->cards.empty()) throw out_of_range("Empty Pile"); */
/*   return this->cards.back(); */
/* } */

/* Card Pile::pop() { */
/*   if (this->cards.empty()) throw out_of_range("Empty Pile"); */

/*   Card card = this->cards.back(); */
/*   this->cards.pop_back(); */
/*   return card; */
/* } */

/* vector<Card> Pile::take(int n) { */
/*   // So first off, if there's not enough, you can't take. */
/*   // Second off, we need a way to separate "visible" cards */
/*   // from "invisible"//unrevealed cards */
/*   if (this->cards.empty()) throw out_of_range("Empty Pile"); */

/* } */


Foundation::Foundation() {}; // default - nothing needs to be set

/* Card Foundation::peek() { */
/*   if */ 
/* } */
