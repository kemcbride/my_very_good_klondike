#ifndef PILE_H
#define PILE_H

#include <optional>
#include <vector>

#include "card.h"
#include "deck.h"
#include "run.h"

// a Pile is a stack of cards that you can push and pop from.
// However, it works wayy differently than a foundation.
// The pile is probably where the business logic of what moves
// are viable lives.
class Pile {
private:
  void clear_empty_run();

public:
  std::vector<Run> runs;

  // TODO: Not sure which I want to use...
  Pile(std::vector<Card>);
  Pile(std::vector<Run>);
  Pile(Deck &, int);

  std::optional<Card> peek();
  std::optional<Run> peek(unsigned int); // take a look at the top N cards
  std::optional<Run> pop();              // remove the top card from the top run
  std::optional<Run>
  take(unsigned int); // take the top N visible cards (if they are movable)
  void put(
      std::vector<Card>); // add a stack of viable cards to the top of the pile
  void put(Run);          // add a stack of viable cards to the top of the pile
  void put(Card);         // add just one card (like, from stock or fdn)
  int reveal_top();      // for when a move is done, reveal the top of the pile
  bool isEmpty();

  std::string toString();
};

#endif // PILE_H
