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
const std::vector<suits> SuitEnumList = {clubs, spades, hearts, diamonds};

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
  const colors getColor() const;
  const suits getSuit() const;
  std::string toString() const;

  friend bool operator<=>(Suit const& a, Suit const& b) = default;
  friend bool operator==(Suit const& a, Suit const& b) = default;
};

std::ostream& operator<<(std::ostream& os, Suit& obj);

class Rank {
 protected:
  int value;

 public:
  Rank();
  Rank(int);
  int getValue() const;
  std::string toString();
  operator int();

  friend bool operator<=>(Rank const& a, Rank const& b) = default;
  friend bool operator==(Rank const& a, Rank const& b) = default;
};
std::ostream& operator<<(std::ostream& os, Rank& obj);

class Card {
 private:
  Rank rank;
  Suit suit;

 public:
  Card();
  Card(Rank, Suit);
  Card(int, int);
  const Suit getSuit() const;
  Rank getRank() const;
  colors getColor() const;
  std::string toString();

  friend bool operator<=>(Card const& a, Card const& b) = default;
  friend bool operator==(Card const& a, Card const& b) = default;
};
std::ostream& operator<<(std::ostream& os, Card& obj);

#endif  // CARD_H
