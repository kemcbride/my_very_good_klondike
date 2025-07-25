#include "lib/deck.h"

#include <gtest/gtest.h>

#include <exception>
#include <vector>

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

  // Valid, and strange: (maybe should disallow)
  // Because draw just works by iterating through the deck,
  // you can draw more than is in the deck.
  d.draw(60);

  // Now, a negative draw probably fails because you can't have a
  // vector with negative size.
  EXPECT_THROW(d.draw(-1), std::exception);
}

TEST(DeckTest, ShuffleConsistency) {
  // This test is aimed particularly at showing
  // that shuffle is actually yielding a different
  // order of cards.
  std::random_device rd;
  std::mt19937 g(rd());

  Deck d(g);
  d.update_seed(1);
  d.shuffle();

  vector<Card> d1 = d.draw(1);

  d.update_seed(2);
  d.shuffle();
  d.reset_index();
  vector<Card> d2 = d.draw(1);

  cerr << d1.at(0) << ", " << d2.at(0) << endl;
  EXPECT_NE(d1.at(0), d2.at(0));
}

TEST(DeckTest, ImplicitBehavior) {
  // This test is aimed at showing how draw behaves -
  // notably how you can draw when there's nothing left,
  // and how resets or shuffles interact with draws.
  // Additionally - none of the Deck's methods are intended to throw!

  // Mostly, how the deck has actually very little in the way
  // of guarantees.
  std::random_device rd;
  std::mt19937 g(rd());
  Deck d(g);

  // Because sort() is called in its constructor, a Deck gets the
  // standard 52 cards in some order.
  EXPECT_EQ(d.cards.size(), 52);
  vector<Card> d1 = d.draw(1);
  EXPECT_EQ(d1.at(0), Card(1, clubs));

  // The index is the only way we manage "how many cards have been taken"
  d.reset_index();

  vector<Card> fullDraw = d.draw(100);
  EXPECT_EQ(fullDraw.size(), 52);

  vector<Card> moreDraw = d.draw(10);
  EXPECT_EQ(moreDraw.size(), 0);

  // Shuffling doesn't affect the index!
  d.shuffle();
  moreDraw = d.draw(10);
  EXPECT_EQ(moreDraw.size(), 0);
}
