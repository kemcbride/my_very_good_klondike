#ifndef RUN_H
#define RUN_H

#include <exception>
#include <optional>
#include <string>
#include <vector>

#include "card.h"

class Run {
private:
  bool revealed = false;

public:
  std::vector<Card> cards;
  bool isRevealed();

  // default constructor is fine for this case
  Run(); // empty run - useful in case of discovering isStuck() state

  Run(Card);
  Run(std::vector<Card>);
  Run(Run, unsigned int); // same energy as Run take(int)
  std::vector<Card> view();
  std::optional<Card> peek();
  Run take(unsigned int);      // you can create a run from another run
  void put(Run);               // you can add another run to a run
  void put(std::vector<Card>); // you can just try to add cards
  void put(Card);              // you can just try to add a card

  bool canAdd(Card);
  bool canAdd(Run);

  int reveal();
  void hide();

  std::string toString();
};

#endif
