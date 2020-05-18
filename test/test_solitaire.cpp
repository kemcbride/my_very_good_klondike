#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "../Catch2/single_include/catch2/catch.hpp"

#include <iostream>
#include <cstdlib>
#include <ctime>

#include "../lib/foundation.h"
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

    // Now that they have the 2nd cards, try to re-add correct suit 1s - should FAIL
    for (unsigned int i=0; i < fdns.size(); ++i) {
      Foundation f = fdns.at(i);
      Card c = valid_first_pushes.at(i);
      REQUIRE_FALSE(f.canPush(c));
      REQUIRE_THROWS(f.push(c));
    }

  }
}
