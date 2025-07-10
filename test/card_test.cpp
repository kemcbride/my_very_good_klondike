#include <gtest/gtest.h>

#include <cstdlib>
#include <ctime>
#include <exception>
#include <iostream>
#include <ranges>
#include <string>

#include "lib/card.h"

using namespace std;

TEST(CardTest, Suit) {
  // Because suits is an enum, you can technically just pass in ints - 
  // but ints outside of 0-3 are all invalid.
  EXPECT_THROW(Suit(-1), std::exception);
  EXPECT_THROW(Suit(4), std::exception);

  Suit s(spades);
  EXPECT_EQ(s.getColor(), black);
  Suit c(clubs);
  EXPECT_EQ(c.getColor(), black);
  Suit h(hearts);
  EXPECT_EQ(h.getColor(), red);
  Suit d(diamonds);
  EXPECT_EQ(d.getColor(), red);
}

TEST(CardTest, Rank) {
  EXPECT_THROW(Rank(-1), invalid_argument);
  EXPECT_THROW(Rank(0), invalid_argument);
  EXPECT_THROW(Rank(14), invalid_argument);

  EXPECT_EQ(Rank(11), 11);
  EXPECT_EQ(Rank(3), 3);

  // Unexpected but possible behavior: adding ranks together
  EXPECT_EQ(Rank(3) + Rank(10), 13);

  // Unexpected but possible - you can get a value out of the addition,
  // even if that result is technically an invalid rank
  EXPECT_EQ(Rank(5) + Rank(10), 15);

  // I dunno, some expectations on the length of the string representation?
  for (auto i : views::iota(1, 13)) {
    Rank r(i);
    string r_string = r.toString();
    if (i == 10) {
      EXPECT_EQ(r_string.size(), 2);
    } else {
      EXPECT_EQ(r_string.size(), 1);
    }
  }
}

TEST(CardTest, Card) {
  EXPECT_THROW(Card(-1, 0), std::exception);
  EXPECT_THROW(Card(0, 0), std::exception);
  EXPECT_THROW(Card(0, -1), std::exception);
  EXPECT_THROW(Card(14, 0), std::exception);
  EXPECT_THROW(Card(0, 5), std::exception);

  Card c;
  Card d(5, hearts);
  Card e(1, spades);

  EXPECT_NE(c, d);
  EXPECT_EQ(c, e);
}
