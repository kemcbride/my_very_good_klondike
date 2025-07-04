#include <gtest/gtest.h>

#include <iostream>
#include <cstdlib>
#include <ctime>

#include "lib/foundation.h"
#include "lib/pile.h"
#include "lib/run.h"
#include "lib/card.h"

using namespace std;

// Demonstrate some basic assertions.
TEST(HelloTest, BasicAssertions) {
	// Expect two strings not to be equal.
	EXPECT_STRNE("hello", "world");
	// Expect equality.
	EXPECT_EQ(7 * 6, 42);
}

TEST(FoundationTest, BasicAssertions) {
  srand((unsigned) time(0));
  vector<Card> valid_first_pushes;
  for (auto s : SuitEnumList) {
    Card c = Card(1, s);
    valid_first_pushes.push_back(c);
  }
  vector<Card> valid_second_pushes;
  for (auto s : SuitEnumList) {
    Card c = Card(2, s);
    valid_second_pushes.push_back(c);
  }
}
