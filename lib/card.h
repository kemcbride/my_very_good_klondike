/* card.h
 * contains some stuff to define cards
 * @kemcbride/@ke2mcbri 2020
 */
/* Is this how header files work??? */
#ifndef CARD_H
#define CARD_H

#include <iostream>
#include <string>
#include <vector>

enum suits {
  clubs,
  spades,
  hearts,
  diamonds,
};
const std::vector<suits> SuitEnumList = { clubs, spades, hearts, diamonds };

enum colors {
  red,
  black,
};

class Suit {
private:
  suits s;

public:
  Suit();
  Suit(suits);
  Suit(unsigned int);
  colors getColor();
  suits getSuit();
  std::string toString();
};
std::ostream &operator<<(std::ostream &os, Suit &obj);
bool operator==(Suit a, Suit b);

class Rank {
protected:
  int value;

public:
  Rank();
  Rank(int);
  int getValue();
  std::string toString();
  operator int();
};
std::ostream &operator<<(std::ostream &os, Rank &obj);

class Card {
private:
  Rank rank;
  Suit suit;

public:
  Card();
  Card(Rank, Suit);
  Card(int, int);
  Suit getSuit();
  Rank getRank();
  colors getColor();
  std::string toString();
};
std::ostream &operator<<(std::ostream &os, Card &obj);

#endif // CARD_H
