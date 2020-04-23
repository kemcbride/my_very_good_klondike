#ifndef BOARD_H
#define BOARD_H

#include <algorithm> // random_shuffle
#include <iostream>
#include <string>
#include <exception>
#include <optional>
#include <vector>

#include "deck.h"
#include "card.h"


class Run {
  private:
    bool revealed = false;
    bool canAdd(Run);
    bool canAdd(std::vector<Card>);
    bool canAdd(Card);

  public:
    std::vector<Card> cards;
    bool isRevealed();

    Run(Card);
    Run(std::vector<Card>);
    std::vector<Card> view();
    Run take(int); // you can create a run from another run
    void put(Run); // you can add another run to a run
    void put(std::vector<Card>); // you can just try to add cards
    void put(Card); // you can just try to add a card

    void reveal();
    void hide();
};

// a Pile is a stack of cards that you can push and pop from.
// However, it works wayy differently than a foundation.
// The pile is probably where the business logic of what moves
// are viable lives.
class Pile {
  public:
    std::vector<Run> runs;

    // TODO: Not sure which I want to use...
    Pile(std::vector<Card>);
    Pile(std::vector<Run>);
    Pile(Deck, int);

    std::optional<Card> peek();
    std::optional<Card> pop(); // remove the top card
    Run take(int); // take the top N visible cards (if they are movable)
    void put(std::vector<Card>); // add a stack of viable cards to the top of the pile
};

// The Tableau consists of 7 piles
class Tableau {
  public:
    std::vector<Pile> piles;

    Tableau(Deck);
};

// The stock in my world is a circular buffer that can pop elements
class Stock {
  private:
    int index;
    void update_index();

  public:
    std::vector<Card> cards;

    Stock(Deck);

    std::optional<Card> peek();
    std::optional<Card> pop();
    void next();
};

// The foundation is a ordered stack of Cards of the same suit
class Foundation {
  public:
    std::optional<Suit> suit;
    std::vector<Card> cards;

    Foundation();

    std::optional<Card> peek();
    std::optional<Card> pop();
    void push(Card);
};

class Board {
  private:
    bool isSolved(); // check if the game is "Solved"/"Solvable"

  public:
    Tableau tableau;
    Stock stock;
    std::vector<Foundation> foundations;

    Board(Deck);
};

#endif // BOARD_H
