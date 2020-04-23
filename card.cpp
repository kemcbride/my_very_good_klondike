/* card.cpp 
 * contains some stuff to define cards and a deck for solitaire play.
 * klondike specfiically, for now.
 * @kemcbride/@ke2mcbri 2020
 */
#include "card.h"

#define REDSTR "\033[1;31m"
#define RESETSTR "\033[0m"

using namespace std;



//enumeration types (both scoped and unscoped) can have overloaded operators
std::ostream& operator<<(std::ostream& os, suits s)
{
    switch(s)
    {
      // ♠  ♥  ♣  ♢  ♦
        case clubs   : os << "♣";    break;
        case spades   : os << "♠";    break;
        case hearts: os << "♥"; break;
        case diamonds : os << "♦";  break;
        default    : os.setstate(std::ios_base::failbit);
    }
    return os;
};


Suit::Suit() {
  this->s = spades;
};
Suit::Suit(suits su) {
  this->s = su;
};

colors Suit::getColor() {
  if (this->s == spades || this->s == clubs)
    return black;
  return red;
};
suits Suit::getSuit() {
  return this->s;
};

ostream& operator<<(ostream& os, Suit& obj) {
  if (obj.getColor() == red) {
    return os << REDSTR << obj.getSuit() << RESETSTR;
  }
  return os << obj.getSuit();
};

Rank::Rank() { this->value = 1;};
Rank::Rank(int val) {
  if (val < 1 || val > 13) {
    throw invalid_argument("Card Ranks go from 1 to 13, invalid number: " + val);
  }
  this->value = val;
};
int Rank::getValue() {
  return this->value;
}

ostream& operator<<(ostream& os, Rank& obj) {
  int val = obj.getValue();
  switch (val) {
    case 1:
      return os << "A";
      break;
    case 11:
      return os << "J";
      break;
    case 12:
      return os << "Q";
      break;
    case 13:
      return os << "K";
      break;
    default:
      break;
  }
  return os << val;
};

Card::Card() : rank(1), suit(spades) {};
Card::Card(Rank r, Suit s) : rank(r), suit(s) {};
Card::Card(int r, int s) {
  this->rank = Rank(r);
  suits su = static_cast<suits>(s);
  this->suit = Suit(su);
}

Suit Card::getSuit() {
  return this->suit;
}

Rank Card::getRank() {
  return this->rank;
}

ostream& operator<<(ostream& os, Card& obj) {
  Suit s = obj.getSuit();
  Rank r = obj.getRank();
  if (s.getColor() == red) {
    return os << REDSTR << r << s << RESETSTR;
  }
  return os << r << s;
};
