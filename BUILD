cc_binary(
    name = "bz_solitaire",
    srcs = [
	    "bin/solitaire.cpp",
	    "lib/board.h",
	    "lib/board.cpp",
	    "lib/card.h",
	    "lib/card.cpp",
	    "lib/command.h",
	    "lib/command.cpp",
	    "lib/deck.h",
	    "lib/deck.cpp",
	    "lib/foundation.h",
	    "lib/foundation.cpp",
	    "lib/location.h",
	    "lib/location.cpp",
	    "lib/move.h",
	    "lib/move.cpp",
	    "lib/move_cmd.h",
	    "lib/move_cmd.cpp",
	    "lib/pile.h",
	    "lib/pile.cpp",
	    "lib/run.h",
	    "lib/run.cpp",
	    "lib/stock.h",
	    "lib/stock.cpp",
	    "lib/tableau.h",
	    "lib/tableau.cpp",
    ],
    copts = [
        "--std=c++20",
    ],
    deps = [
        "@gflags//:gflags",
    ]
)

cc_test(
    name = "gtest_card",
    srcs = [
        "test/card_test.cpp",
        "lib/card.h",
        "lib/card.cpp",
        ],
    copts = [
        "--std=c++20",
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
    copts = [
        "--std=c++20",
    ],
    deps = [
        ":deck",
        "@googletest//:gtest",
        "@googletest//:gtest_main",
    ]
)

cc_library(
    name = "card",
    srcs = ["lib/card.h", "lib/card.cpp"],
)

cc_library(
    name = "location",
    srcs = ["lib/location.h", "lib/location.cpp"],
)

cc_library(
    name = "deck",
    srcs = ["lib/deck.h", "lib/deck.cpp"],
    deps = [":card"],
)
