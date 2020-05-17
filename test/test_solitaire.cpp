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

  SECTION( "Foundation initialization" ) {
    Foundation f = Foundation();
    REQUIRE_THROWS( f.getSuit() );
  }
  SECTION( "Foundation first pushes" ) {
    vector<Card> valid_first_pushes;
    cout << "Valid first push cards: ";
    for (auto s : SuitEnumList) {
      Card c = Card(1, s);
      valid_first_pushes.push_back(c);
      cout << c;
      if (s != SuitEnumList.back()) {
        cout << ", ";
      } else {
        cout << endl;
      }
    }
    vector<Card> invalid_first_pushes;
    cout << "Invalid first push cards: ";
    for (auto s : SuitEnumList) {
      unsigned int i = (rand() % 12 + 2);
      Card c = Card(i, s);
      invalid_first_pushes.push_back(c);
      cout << c;
      if (s != SuitEnumList.back()) {
        cout << ", ";
      } else {
        cout << endl;
      }
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
}
