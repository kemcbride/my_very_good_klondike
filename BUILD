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
    deps = [
        "@gflags//:gflags",
    ]
)

cc_binary(
    name = "bz_solitaire_gtest",
    srcs = [
        "test/card_test.cpp",
        "lib/card.h",
        "lib/card.cpp",
        ],
    deps = [
        "@googletest//:gtest",
    ]
)
