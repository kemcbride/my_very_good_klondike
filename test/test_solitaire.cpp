#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "../Catch2/single_include/catch2/catch.hpp"

#include <iostream>
#include <cstdlib>
#include <ctime>

#include "../lib/foundation.h"
#include "../lib/run.h"
#include "../lib/card.h"

using namespace std;

TEST_CASE( "Foundations work as expected", "[foundation]" ) {
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
  

  SECTION( "Foundation initialization" ) {
    Foundation f = Foundation();
    REQUIRE_THROWS( f.getSuit() );
  }
  SECTION( "Foundation first pushes" ) {
    vector<Card> invalid_first_pushes;
    for (auto s : SuitEnumList) {
      unsigned int i = (rand() % 12 + 2);
      Card c = Card(i, s);
      invalid_first_pushes.push_back(c);
    }
    for (auto c : valid_first_pushes) {
      Foundation f = Foundation();
      REQUIRE(f.canPush(c));
      REQUIRE_NOTHROW(f.push(c));
    }
    for (auto c : invalid_first_pushes) {
      Foundation f = Foundation();
      REQUIRE_FALSE(f.canPush(c));
      REQUIRE_THROWS(f.push(c));
    }
  }

  SECTION( "Foundation Nth (2nd) pushes" ) {
    // Set them each up with expected first pushes
    vector<Foundation> fdns(4, Foundation());
    for (unsigned int i=0; i < fdns.size(); ++i) {
      fdns.at(i).push(valid_first_pushes.at(i));
    }
    // Make sure that re-adding the same cards does NOT work
    for (unsigned int i=0; i < fdns.size(); ++i) {
      Foundation f = fdns.at(i);
      Card c = valid_first_pushes.at(i);
      REQUIRE_FALSE(f.canPush(c));
      REQUIRE_THROWS(f.push(c));
    }
    // Make sure that pushing the WRONG 2nd pushes (wrong suit) does NOT work
    for (unsigned int i=0; i < fdns.size(); ++i) {
      Foundation f = fdns.at(i);
      for (unsigned int j=1; j < fdns.size(); ++j) {
        Card c = valid_second_pushes.at((i+j) % 4);
        REQUIRE_FALSE(f.canPush(c));
        REQUIRE_THROWS(f.push(c));
      }
    }
    // Let's try to push their expected 2nd pushes
    for (unsigned int i=0; i < fdns.size(); ++i) {
      Foundation f = fdns.at(i);
      Card c = valid_second_pushes.at(i);
      REQUIRE(f.canPush(c));
      REQUIRE_NOTHROW(f.push(c));
    }

    // Now that they have the 2nd cards, try to re-add correct suit 1s - should FAILt
    for (unsigned int i=0; i < fdns.size(); ++i) {
      Foundation f = fdns.at(i);
      Card c = valid_first_pushes.at(i);
      REQUIRE_FALSE(f.canPush(c));
      REQUIRE_THROWS(f.push(c));
    }

  }
}

TEST_CASE( "Runs work as expected", "[run]" ) {
  SECTION( "Run initialization tests" ) {
    REQUIRE_NOTHROW(Run());
    // The first 2 suits are black, the second 2 suits are red
    // Card(rank, suit)
    Card c(1, 3); // red ace
    Run r(c);
    REQUIRE_NOTHROW(r);
    REQUIRE(!r.isRevealed());
    vector<Card> v;
    r = Run(v);
    REQUIRE_NOTHROW(r); // an empty vector init IS VALID
    REQUIRE(!r.isRevealed());
    v.push_back(c);
    r = Run(v);
    REQUIRE_NOTHROW(r);
    REQUIRE(!r.isRevealed());

    // Invalid run(vector): order-wise
    vector<Card> invalid_run;
    invalid_run.push_back(c);
    invalid_run.push_back(Card(2, 0)); // black 2
    REQUIRE_THROWS(Run(invalid_run));

    invalid_run.clear();
    // Invalid run(vector): suit-wise
    invalid_run.push_back(Card(2, 2)); // red 2
    invalid_run.push_back(c); // red ace
    REQUIRE_THROWS(Run(invalid_run));
  }
  SECTION( "Empty Run push/add tests" ) {
    // TODO: Is this desirable behavior?
    vector<Card> all_kings;
    vector<Card> all_aces;
    for (auto s : SuitEnumList) {
      all_aces.push_back(Card(1, s));
      all_kings.push_back(Card(13, s));
    }
    for (auto c : all_aces) {
      Run r;
      REQUIRE_FALSE(r.canAdd(c));
      REQUIRE_THROWS(r.put(c));
    }
    for (auto c : all_kings) {
      Run r;
      REQUIRE(r.canAdd(c));
      REQUIRE_NOTHROW(r.put(c));
    }
  }
  SECTION( "Populated (single-card) Run push/add tests" ) {
    Card red_2(2, 2);
    Card red_ace = Card(1, 2);
    Card black_ace = Card(1, 1);
    Run has_red_2(red_2);

    REQUIRE_FALSE(has_red_2.canAdd(red_ace));
    REQUIRE_THROWS(has_red_2.put(red_ace));

    REQUIRE(has_red_2.canAdd(black_ace));
    REQUIRE_NOTHROW(has_red_2.put(black_ace));
  }
  // TODO: could also similarly test put with vector of card and put with run, same cases
}
