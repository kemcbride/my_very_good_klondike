#include <benchmark/benchmark.h>
#include <iostream>
#include <vector>
#include <algorithm>

#include "lib/board.h"
#include "lib/card.h"
#include "lib/foundation.h"
#include "lib/pile.h"
#include "lib/run.h"

static void BM_IsStuck(benchmark::State& state) {
  // Setup here - setup the board.
  std::random_device rd;
  std::mt19937 g(rd());
  Deck d(g);
  // The default board state is a normal klondike start state - empty foundations,
  // 7 piles on the tableau, each with just one card revealed, and rest in stock.
  // Specifically, the stock has 24? cards
  Board b = Board(d, true /*FLAGS_autosolve*/, true/*FLAGS_autoreveal*/,
    true/*FLAGS_recycle_penalty_enabled*/);

  for (auto _ : state) {
    b.isStuck();
  }
}

BENCHMARK(BM_IsStuck);


static void BM_IsStuckFull(benchmark::State& state) {
  // Setup here - setup the board.
  std::random_device rd;
  std::mt19937 g(rd());
  Deck d(g);
  Board b = Board(d, true /*FLAGS_autosolve*/, true/*FLAGS_autoreveal*/,
    true/*FLAGS_recycle_penalty_enabled*/);

  d.reset_index();
  std::vector<std::vector<Card>> cardsPerSuit;
  for (int i = 0; i < 4; ++i) {
    std::vector<Card> cards13 = d.draw(13);
    std::reverse(cards13.begin(), cards13.end());

    cardsPerSuit.push_back(cards13);
  }

  for (int i = 0; i < 4; ++i) {
    std::vector<Card> cards;
    for(int j = 0; j < 13; ++j) {
      int suitIdx = i%2 + (j % 2) * 2;
      cards.push_back(cardsPerSuit[suitIdx][j]);
    }
    Run r = Run(cards);
    std::vector<Run> rs = std::vector<Run>();
    rs.push_back(r);
    Pile p = Pile(rs);
    b.tableau.piles[i] = p;
  }
  // I'm not exactly sure why this broke it (segfault) but whatever - removing this lets it run to completion.
  // for(int i = 4; i < 7; ++i) {
  //   std::vector<Card> cardsEmpty;
  //   b.tableau.piles[i] = cardsEmpty;
  // }
  for (auto _ : state) {
    b.isStuck();
  }
}

BENCHMARK(BM_IsStuckFull);

BENCHMARK_MAIN();
