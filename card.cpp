/* card.cpp 
 * contains some stuff to define cards and a deck for solitaire play.
 * klondike specfiically, for now.
 * @kemcbride/@ke2mcbri 2020
 */
#include "card.h"

#define REDSTR "\033[31m"
#define RESETSTR "\033[0m"

using namespace std;


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
static const char * EnumStrings[] = { "♣", "♠", "♥", "♦" };
const char * suitStr( int enumVal ) {
  return EnumStrings[enumVal];
}


// Suit
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
  return os << obj.toString();
};

string Suit::toString() {
  if (this->getColor() == red) {
    return string(string(REDSTR) + suitStr(this->getSuit()) + string(RESETSTR));
  }
  return suitStr(this->getSuit());

}

bool operator==(Suit a, Suit b) {
  return a.getSuit() == b.getSuit();
};

// Rank
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
  return os << obj.toString();
};

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

Rank::operator int() {
  return this->getValue();
}

// Card
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

colors Card::getColor() {
  return this->suit.getColor();
}

string Card::toString() {
  return this->getSuit().toString() + this->getRank().toString();
};

ostream& operator<<(ostream& os, Card& obj) {
  Suit s = obj.getSuit();
  Rank r = obj.getRank();
  if (s.getColor() == red) {
    return os << REDSTR << r << s << RESETSTR;
  }
  return os << r << s;
};
