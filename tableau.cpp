/* tableau.cpp 
 * contains some stuff to define tableau for solitaire play.
 * klondike specfiically, for now.
 * @kemcbride/@ke2mcbri 2020
 */

#include "tableau.h"

using namespace std;

Tableau::Tableau(Deck d) {
  vector<Pile> my_piles;
  for (int i=0; i < 7; ++i) {
    my_piles.push_back(Pile(d, i+1));
  }
  this->piles = my_piles;
}

