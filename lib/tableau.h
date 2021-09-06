#ifndef TABLEAU_H
#define TABLEAU_H

#include <vector>

#include "deck.h"
#include "pile.h"

// The Tableau consists of 7 piles
class Tableau {
private:
  bool show_labels = false;

public:
  Tableau() = delete;
  std::vector<Pile> piles;

  Tableau(Deck &);
  std::string toString();

  void toggleLabels();
  int reveal_top_runs();

  bool isEmpty();
};

#endif // TABLEAU_H
