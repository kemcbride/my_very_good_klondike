#include <benchmark/benchmark.h>

#include "lib/board.h"
#include "lib/card.h"
#include "lib/foundation.h"
#include "lib/pile.h"
#include "lib/run.h"

// static void BM_IsStuck(benchmark::State& state) {
//   // Setup here - setup the board.
//
//   for (auto _ : state) {
//     std::random_device rd;
//     std::mt19937 g(rd());
//     Deck d(g);
//     Board b = Board(d, true /*FLAGS_autosolve*/, true/*FLAGS_autoreveal*/,
//         true/*FLAGS_recycle_penalty_enabled*/);
//     b.isStuck();
//   }
// }

static void BM_Math(benchmark::State& state) {
  for (auto _ : state) {
    300+400;
  }
}

BENCHMARK(BM_Math);

BENCHMARK_MAIN();
