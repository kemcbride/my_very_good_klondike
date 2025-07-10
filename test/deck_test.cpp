#include <gtest/gtest.h>

#include <exception>
#include <vector>

#include "lib/deck.h"

using namespace std;

TEST(DeckTest, Basics) {
  std::random_device rd;
  std::mt19937 g(rd());

  Deck d(g);
  Deck e(g);

  vector<Card> d1 = d.draw(1);
  vector<Card> e1 = e.draw(1);
  EXPECT_EQ(d1, e1);

  
  d.shuffle();
  e.shuffle();
  d1 = d.draw(1);
  e1 = e.draw(1);
  EXPECT_EQ(d1, e1);

  // Valid, but not meaningful.
  // Show that doing a draw(0) doesn't mess up the current_idx
  d.draw(0);
  d1 = d.draw(1);
  e1 = e.draw(1);
  EXPECT_EQ(d1, e1);
}
