#ifndef TABLEAU_H
#define TABLEAU_H

#include <vector>

#include "deck.h"
#include "pile.h"


// The Tableau consists of 7 piles
class Tableau {
  private:

  public:
    Tableau() = delete;
    std::vector<Pile> piles;

    Tableau(Deck);
};

#endif // TABLEAU_H
