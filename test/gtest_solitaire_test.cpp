#include <gtest/gtest.h>

#include <cstdlib>
#include <ctime>
#include <exception>
#include <iostream>

#include "lib/foundation.h"
#include "lib/pile.h"
#include "lib/run.h"
#include "lib/card.h"

using namespace std;

TEST(FoundationTest, Initialization) {
  Foundation f = Foundation();
  EXPECT_THROW( f.getSuit(), std::exception );
}


TEST(FoundationTest, FirstSecondThirdPushes) {
  srand((unsigned) time(0));
  vector<Card> valid_first_pushes;
  for (auto s : SuitEnumList) {
    Card c = Card(1, s);
    valid_first_pushes.push_back(c);
  }
  vector<Card> invalid_first_pushes;
  for (auto s : SuitEnumList) {
    unsigned int i = (rand() % 12 + 2);
    Card c = Card(i, s);
    invalid_first_pushes.push_back(c);
  }


  Foundation f = Foundation();

  for (auto c : valid_first_pushes) {
    Foundation f = Foundation();
    EXPECT_TRUE(f.canPush(c));
    f.push(c); // implicitly - expected to not throw
  }
  for (auto c : invalid_first_pushes) {
    Foundation f = Foundation();
    EXPECT_FALSE(f.canPush(c));
    EXPECT_THROW(f.push(c), std::exception);
  }

  // Foundation Nth (2nd) pushes
  vector<Card> valid_second_pushes;
  for (auto s : SuitEnumList) {
    Card c = Card(2, s);
    valid_second_pushes.push_back(c);
  }

  // Set them each up with expected first pushes
  vector<Foundation> fdns(4, Foundation());
  for (unsigned int i=0; i < fdns.size(); ++i) {
    fdns.at(i).push(valid_first_pushes.at(i));
  }
  // Make sure that re-adding the same cards does NOT work
  for (unsigned int i=0; i < fdns.size(); ++i) {
    Foundation f = fdns.at(i);
    Card c = valid_first_pushes.at(i);
    EXPECT_FALSE(f.canPush(c));
    EXPECT_THROW(f.push(c), std::exception);
  }
  // Make sure that pushing the WRONG 2nd pushes (wrong suit) does NOT work
  for (unsigned int i=0; i < fdns.size(); ++i) {
    Foundation f = fdns.at(i);
    for (unsigned int j=1; j < fdns.size(); ++j) {
      Card c = valid_second_pushes.at((i+j) % 4);
      EXPECT_FALSE(f.canPush(c));
      EXPECT_THROW(f.push(c), std::exception);
    }
  }
  // Let's try to push their expected 2nd pushes
  for (unsigned int i=0; i < fdns.size(); ++i) {
    Foundation f = fdns.at(i);
    Card c = valid_second_pushes.at(i);
    EXPECT_TRUE(f.canPush(c));
    f.push(c);
  }

  // Now that they have the 2nd cards, try to re-add correct suit 1s - should FAIL
  for (unsigned int i=0; i < fdns.size(); ++i) {
    Foundation f = fdns.at(i);
    Card c = valid_first_pushes.at(i);
    EXPECT_FALSE(f.canPush(c));
    EXPECT_THROW(f.push(c), std::exception);
  }
}


TEST(RunTest, BasicAssertions) {
  solitaire::Run();
  // The first 2 suits are black, the second 2 suits are red
  // Card(rank, suit)
  Card c(1, 3); // red ace
  solitaire::Run r(c);
  EXPECT_TRUE(!r.isRevealed());
  vector<Card> v;
  r = solitaire::Run(v);
  EXPECT_TRUE(!r.isRevealed());
  v.push_back(c);
  r = solitaire::Run(v);
  EXPECT_TRUE(!r.isRevealed());

  // Invalid run(vector): order-wise
  vector<Card> invalid_run;
  invalid_run.push_back(c);
  invalid_run.push_back(Card(2, 0)); // black 2 (clubs)
  EXPECT_THROW(solitaire::Run badrun(invalid_run), std::runtime_error);

  invalid_run.clear();
  // Invalid run(vector): suit-wise
  invalid_run.push_back(Card(2, 2)); // red 2
  invalid_run.push_back(c); // red ace
  EXPECT_THROW(solitaire::Run badrun(invalid_run), std::exception);

  // "Empty Run push/add tests"  {
  // TODO: Is this desirable behavior?
  vector<Card> all_kings;
  vector<Card> all_aces;
  for (auto s : SuitEnumList) {
    all_aces.push_back(Card(1, s));
    all_kings.push_back(Card(13, s));
  }
  for (auto c : all_aces) {
    solitaire::Run r;
    EXPECT_FALSE(r.canAdd(c));
    EXPECT_THROW(r.put(c), std::exception);
  }
  for (auto c : all_kings) {
    solitaire::Run r;
    EXPECT_TRUE(r.canAdd(c));
    r.put(c);
  }

  // SECTION( "Populated (single-card) Run push/add tests" ) {
  Card red_2(2, 2);
  Card red_ace = Card(1, 2);
  Card black_ace = Card(1, 1);
  solitaire::Run has_red_2(red_2);

  EXPECT_FALSE(has_red_2.canAdd(red_ace));
  EXPECT_THROW(has_red_2.put(red_ace), std::exception);

  EXPECT_TRUE(has_red_2.canAdd(black_ace));
  has_red_2.put(black_ace);

  // TODO: could also similarly test put with vector of card and put with run, same cases
  // SECTION( "Run put tests" ) {
  vector<Card> red_cards;
  vector<Card> black_cards;
  for (int i = 1; i < 14; ++i) {
    red_cards.push_back(Card(i, 2));
    black_cards.push_back(Card(i, 0));
  }
  vector<Card> some_cards_run;
  vector<Card> same_cards_but_less_run;
  for (int i = 0; i < 4; ++i) {
    vector<Card> src_deck = black_cards;
    if (i % 2 == 0) {
      src_deck = red_cards; // on even, it's red.
    }
    some_cards_run.push_back(src_deck.at(12 - i));
    if (i < 3) {
      same_cards_but_less_run.push_back(src_deck.at(12 - i));
    }
  }
  solitaire::Run r1(some_cards_run);
  solitaire::Run r2(same_cards_but_less_run);
  r1.reveal();
  r2.reveal();

  solitaire::Run r3(r1.view().back());
  r2.put(r3);

  r3 = r1.take(1);
  EXPECT_TRUE(r3.cards.size() == 1);
}

TEST(PileTest, BasicAssertions) {
  // SECTION( "Peek(n) takes right number of cards" ) {
  vector<Card> red_cards;
  vector<Card> black_cards;
  for (int i = 1; i < 14; ++i) {
    red_cards.push_back(Card(i, 2));
    black_cards.push_back(Card(i, 0));
  }
  vector<Card> some_cards_run;
  for (int i = 0; i < 4; ++i) {
    vector<Card> src_deck = black_cards;
    if (i % 2 == 0) {
      src_deck = red_cards; // on even, it's red.
    }
    some_cards_run.push_back(src_deck.at(12 - i));
  }
  solitaire::Run example_src_run(some_cards_run);
  vector<solitaire::Run> pile_setup;
  pile_setup.push_back(example_src_run);
  Pile p(pile_setup);
  EXPECT_EQ(p.runs.back().cards.size(), 4);

  optional<solitaire::Run> maybe_run = p.peek(2);
  EXPECT_TRUE(maybe_run.has_value());
  EXPECT_EQ(maybe_run.value().cards.size(), 2);
}
