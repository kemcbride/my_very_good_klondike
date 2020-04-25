/* tableau.cpp 
 * contains some stuff to define tableau for solitaire play.
 * klondike specfiically, for now.
 * @kemcbride/@ke2mcbri 2020
 */

#include "tableau.h"

using namespace std;

Tableau::Tableau(Deck &d) {
  vector<Pile> my_piles;
  for (int i=0; i < 7; ++i) {
    my_piles.push_back(Pile(d, i+1));
  }
  this->piles = my_piles;
}

string Tableau::toString() {
  string tableau_str;
  for (unsigned int i = 0; i < this->piles.size(); ++i) {
    if (this->show_labels) {
      tableau_str += "p" + i;
      tableau_str += " ";
    }
    tableau_str += this->piles.at(i).toString();
    if (i != this->piles.size() -1) {
      tableau_str += '\n';
    }
  }
  return tableau_str;
}

void Tableau::toggle_labels() {
  this->show_labels = !this->show_labels;
}
