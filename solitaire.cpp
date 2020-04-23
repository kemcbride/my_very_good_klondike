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

  //Now let's just try declaring & instantiating some of these guys
  Foundation f;
  Pile p(deck, 7);
  Stock s(deck);

  Board b(Deck());
	return 0;
}
