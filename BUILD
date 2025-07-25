cc_binary(
    name = "solitaire",
    srcs = [
        "bin/solitaire.cpp",
    ],
    deps = [
        ":board",
        "@gflags//:gflags",
    ]
)

#############################################
### Libraries
#############################################

cc_library(
    name = "board",
    srcs = ["lib/board.h", "lib/board.cpp"],
    deps = [
        ":card",
        ":command",
        ":deck",
        ":foundation",
        ":move",
        ":move_cmd",
        ":pile",
        ":run",
        ":stock",
        ":tableau",
    ],
)

cc_library(
    name = "card",
    srcs = ["lib/card.h", "lib/card.cpp"],
)

cc_library(
    name = "command",
    srcs = ["lib/command.h", "lib/command.cpp"],
)

cc_library(
    name = "deck",
    srcs = ["lib/deck.h", "lib/deck.cpp"],
    deps = [":card"],
)

cc_library(
    name = "foundation",
    srcs = ["lib/foundation.h", "lib/foundation.cpp"],
    deps = [":card"],
)

cc_library(
    name = "location",
    srcs = ["lib/location.h", "lib/location.cpp"],
)

cc_library(
    name = "move",
    srcs = ["lib/move.h", "lib/move.cpp"],
    deps = [":move_cmd", ":run"],
)

cc_library(
    name = "move_cmd",
    srcs = ["lib/move_cmd.h", "lib/move_cmd.cpp"],
    deps = [":command", ":location"],
)

cc_library(
    name = "pile",
    srcs = ["lib/pile.h", "lib/pile.cpp"],
    deps = [":deck", ":run"],
)

cc_library(
    name = "run",
    srcs = ["lib/run.h", "lib/run.cpp"],
    deps = [":card"],
)

cc_library(
    name = "stock",
    srcs = ["lib/stock.h", "lib/stock.cpp"],
    deps = [":card", ":deck"],
)

cc_library(
    name = "tableau",
    srcs = ["lib/tableau.h", "lib/tableau.cpp"],
    deps = [":deck", ":pile", ":run"],
)

#############################################
### Test Targets
#############################################

cc_test(
    name = "gtest_card",
    srcs = [
        "test/card_test.cpp",
        "lib/card.h",
        "lib/card.cpp",
        ],
    deps = [
        ":card",
        "@googletest//:gtest",
        "@googletest//:gtest_main",
    ]
)

cc_test(
    name = "gtest_deck",
    srcs = [
        "test/deck_test.cpp",
        ],
    deps = [
        ":deck",
        "@googletest//:gtest",
        "@googletest//:gtest_main",
    ]
)

cc_test(
    name = "gtest_board",
    srcs = [
        "test/board_test.cpp",
        ],
    deps = [
        ":board",
        "@googletest//:gtest",
        "@googletest//:gtest_main",
    ]
)

cc_test(
    name = "gtest",
    srcs = [
        "test/gtest_solitaire_test.cpp",
    ],
    deps = [
        ":card",
        ":foundation",
        ":pile",
        ":run",
        "@googletest//:gtest",
        "@googletest//:gtest_main",
    ]
)


#############################################
### Benchmark Targets
#############################################

cc_binary(
    name = "isstuck_benchmark",
    srcs = ["benchmark/isstuck_benchmark.cpp"],
    deps = [
        "@google_benchmark//:benchmark_main",
        ":board",
    ],
)
