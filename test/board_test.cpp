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

  // TODO - add "next" to these command lists once it's a thing.
  vector<string> expectedStartCmds{"p1 1 f1", "p1 1 f2", "p1 1 f3",
                                   "p1 1 f4", "p1 1 p7", "p7 1 p2"};

  EXPECT_EQ(b.getLegalCommands(), expectedStartCmds);

  b.move(b.getLegalMoves()[0]);
  cout << b.toString() << endl;
  cout << "First moves: " << endl;
  for (auto m : b.getLegalMoves()) {
    cout << m.toString() << ", ";
  }
  cout << endl;
  cout << "First cmds: " << endl;
  for (auto c : b.getLegalCommands()) {
    cout << c << ", ";
  }
  cout << endl;
  vector<string> expectedSecondCmds{
      "p7 1 p2",
  };

  EXPECT_EQ(b.getLegalCommands(), expectedSecondCmds);
}
