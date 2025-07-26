#include "lib/board.h"

#include <gtest/gtest.h>

#include <exception>
#include <iostream>
#include <vector>

using namespace std;

TEST(BoardTest, HintCorrectness) {
  std::mt19937 g(100);

  Deck d(g);
  Board b(d, true /* auto_solve */, true /*auto_reveal*/,
          true /*recycle enabled*/);

  b.toggleLabels();

  // TODO - change hints to use stable sort (& next always at end)
  set<string> expectedStartCmds{"m p1 1 f1", "m p1 1 f2", "m p1 1 f3", "next",
                                "m p1 1 f4", "m p1 1 p7", "m p7 1 p2"};

  vector<string> lc = b.getLegalCommands();
  set<string> lcs(lc.begin(), lc.end());
  EXPECT_EQ(lcs, expectedStartCmds);

  b.move(b.getLegalMoves()[0]);
  // cout << b.toString() << endl;
  // cout << "First moves: " << endl;
  // for (auto m : b.getLegalMoves()) {
  //   cout << m.toString() << ", ";
  // }
  // cout << endl;
  // cout << "First cmds: " << endl;
  // for (auto c : b.getLegalCommands()) {
  //   cout << c << ", ";
  // }
  // cout << endl;
  set<string> expectedSecondCmds{"m p7 1 p2", "next"};

  lc = b.getLegalCommands();
  lcs = set<string>(lc.begin(), lc.end());
  EXPECT_EQ(lcs, expectedSecondCmds);
}
