/* tableau.cpp
 * contains some stuff to define tableau for solitaire play.
 * klondike specfiically, for now.
 * @kemcbride/@ke2mcbri 2020
 */

#include "tableau.h"

using namespace std;

Tableau::Tableau(Deck &d) {
  vector<Pile> my_piles;
  for (int i = 0; i < 7; ++i) {
    my_piles.push_back(Pile(d, i + 1));
  }
  this->piles = my_piles;
}

string Tableau::toString() {
  string tableau_str;
  for (unsigned int i = 0; i < this->piles.size(); ++i) {
    if (this->show_labels) {
      tableau_str += "p" + to_string(i + 1) + " ";
    }
    tableau_str += this->piles.at(i).toString();
    if (i != this->piles.size() - 1) {
      tableau_str += '\n';
    }
  }
  return tableau_str;
}

void Tableau::toggleLabels() { this->show_labels = !(this->show_labels); }

int Tableau::reveal_top_runs() {
  int num_reveals = 0;
  for (auto &p : this->piles) {
    num_reveals += p.reveal_top();
  }
  return num_reveals;
}

bool Tableau::isEmpty() {
  for (auto p : this->piles) {
    if (!p.isEmpty()) return false;
  }
  return true;
}
