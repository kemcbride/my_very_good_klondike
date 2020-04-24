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

  cout << "Shuffling and drawing top card a few times..." << endl;
  for(int i = 0; i < 5; ++i) {
    deck.shuffle();
    cout << deck.cards.at(0) << endl;
  }

  cout << "Now let's draw a few hands." << endl;
  for(int i=0; i < 3; ++i) {
    for(auto c : deck.draw(5)) {
      cout << c << " ";
    }
    cout << endl;
  }

  deck.reset_index();
  deck.shuffle();

  //Now let's just try declaring & instantiating some of these guys
  Board b(deck);
  cout << b.tableau.piles.front().runs.front().toString() << endl;
	return 0;
}
