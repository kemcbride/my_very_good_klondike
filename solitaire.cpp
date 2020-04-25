/* solitaire.cpp 
 * contains some stuff to define cards and a deck for solitaire play.
 * klondike specfiically, for now.
 * @kemcbride/@ke2mcbri 2020
 */
#include <iostream>

#include "card.h"
#include "deck.h"
#include "board.h"

using namespace std;


int main(int argc, char** argv) {
  Deck deck;
  deck.shuffle();

  // Now let's just try declaring & instantiating some of these guys
  Board b(deck);
  cout << b.tableau.toString() << endl;
	return 0;
}
