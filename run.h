#ifndef RUN_H
#define RUN_H

#include <exception>
#include <vector>
#include <optional>
#include <string>

#include "card.h"


class Run {
  private:
    bool revealed = false;
    bool canAdd(Run);

  public:
    std::vector<Card> cards;
    bool isRevealed();

    Run(Card);
    Run(std::vector<Card>);
    Run(Run, unsigned int); // same energy as Run take(int)
    std::vector<Card> view();
    std::optional<Card> peek();
    Run take(unsigned int); // you can create a run from another run
    void put(Run); // you can add another run to a run
    void put(std::vector<Card>); // you can just try to add cards
    void put(Card); // you can just try to add a card

    bool canAdd(Card);

    void reveal();
    void hide();

    std::string toString();
};

#endif
