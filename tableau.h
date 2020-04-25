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

    Tableau(Deck&);
    std::string toString();

    void toggle_labels();
};

#endif // TABLEAU_H
