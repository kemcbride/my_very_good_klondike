#ifndef RUN_H
#define RUN_H

#include <exception>
#include <vector>

#include "card.h"


class Run {
  private:
    bool revealed = false;
    bool canAdd(Run);
    bool canAdd(Card);

  public:
    std::vector<Card> cards;
    bool isRevealed();

    Run(Card);
    Run(std::vector<Card>);
    std::vector<Card> view();
    Run take(unsigned int); // you can create a run from another run
    void put(Run); // you can add another run to a run
    void put(std::vector<Card>); // you can just try to add cards
    void put(Card); // you can just try to add a card

    void reveal();
    void hide();
};

#endif
