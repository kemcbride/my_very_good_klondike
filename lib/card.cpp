/* card.cpp
 * contains some stuff to define cards and a deck for solitaire play.
 * klondike specfiically, for now.
 * @kemcbride/@ke2mcbri 2020
 */
#include "card.h"

#define REDSTR "\033[31m"
#define RESETSTR "\033[0m"

using namespace std;

std::ostream &operator<<(std::ostream &os, suits s) {
  switch (s) {
      // ♠  ♥  ♣  ♢  ♦
    case clubs:
      os << "♣";
      break;
    case spades:
      os << "♠";
      break;
    case hearts:
      os << "♥";
      break;
    case diamonds:
      os << "♦";
      break;
    default:
      os.setstate(std::ios_base::failbit);
  }
  return os;
};
static const char *EnumStrings[] = {"♣", "♠", "♥", "♦"};
const char *suitStr(int enumVal) { return EnumStrings[enumVal]; }

// Suit
Suit::Suit() : s(spades){};
Suit::Suit(unsigned int i) : s(SuitEnumList.at(i)){};
Suit::Suit(suits su) : s(su){};

const colors Suit::getColor() const {
  if (this->s == spades || this->s == clubs) return black;
  return red;
};
const suits Suit::getSuit() const { return this->s; };

ostream &operator<<(ostream &os, Suit &obj) { return os << obj.toString(); };

string Suit::toString() const {
  if (this->getColor() == red) {
    return string(string(REDSTR) + suitStr(this->getSuit()) + string(RESETSTR));
  }
  return suitStr(this->getSuit());
}

// Rank
Rank::Rank() { this->value = 1; };
Rank::Rank(int val) {
  if (val < 1 || val > 13) {
    throw invalid_argument("Card Ranks go from 1 to 13, invalid number: " +
                           to_string(val));
  }
  this->value = val;
};
int Rank::getValue() const { return this->value; }

ostream &operator<<(ostream &os, Rank &obj) { return os << obj.toString(); };

string Rank::toString() {
  int val = this->getValue();
  string str;
  switch (val) {
    case 1:
      str = "A";
      break;
    case 11:
      str = "J";
      break;
    case 12:
      str = "Q";
      break;
    case 13:
      str = "K";
      break;
    default:
      str = to_string(val);
      break;
  }
  return str;
}

Rank::operator int() { return this->getValue(); }

// Card
Card::Card() : rank(1), suit(spades){};
Card::Card(Rank r, Suit s) : rank(r), suit(s){};
Card::Card(int r, int s) {
  this->rank = Rank(r);
  suits su = static_cast<suits>(s);
  this->suit = Suit(su);
}

const Suit Card::getSuit() const { return this->suit; }

Rank Card::getRank() const { return this->rank; }

colors Card::getColor() const { return this->suit.getColor(); }

string Card::toString() {
  string base_str = this->getRank().toString() + this->getSuit().toString();
  if (this->getSuit().getColor() == red) {
    return REDSTR + base_str + RESETSTR;
  }
  return base_str;
};

ostream &operator<<(ostream &os, Card &obj) { return os << obj.toString(); };
