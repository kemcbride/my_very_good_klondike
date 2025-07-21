CC=clang++
CXX=clang++
CC_FLAGS=-Wall -Wextra -pthread -I. --std=c++20 -stdlib=libc++
# On osx using homebrew, may need to add to CC_FLAGS:
# -I$(shell (brew --prefix))/include -L$(shell (brew --prefix))/lib

GFLAGS_IDIR = /usr/include/gflags
GTEST_IDIR = /usr/include/gtest
GTEST_SRCDIR = /usr/src/gtest
GTEST_HEADERS = $(GTEST_IDIR)/*.h \
				$(GTEST_IDIR)/internal/*.h
GTEST_SRCS_ = $(GTEST_SRCDIR)/src/*.cc $(GTEST_SRCDIR)/src/*.h $(GTEST_HEADERS)
BENCHMARK_IDIR = /usr/include/benchmark

CPUS ?= $(shell (nproc --all || sysctl -n hw.ncpu) 2>/dev/null || echo 1)
MAKEFLAGS += --jobs=$(CPUS)

objects = objects/dbg/card.o objects/dbg/deck.o objects/dbg/pile.o objects/dbg/run.o \
	  objects/dbg/foundation.o objects/dbg/stock.o objects/dbg/tableau.o objects/dbg/board.o \
	  objects/dbg/command.o objects/dbg/move_cmd.o objects/dbg/hint_cmd.o \
	  objects/dbg/move.o objects/dbg/location.o

prof_objects = objects/prof/card.o objects/prof/deck.o objects/prof/pile.o \
	  objects/prof/run.o objects/prof/foundation.o objects/prof/stock.o \
		objects/prof/tableau.o objects/prof/board.o objects/prof/command.o \
		objects/prof/move_cmd.o objects/prof/hint_cmd.o objects/prof/move.o \
		objects/prof/location.o

opt_objects = objects/opt/card.o objects/opt/deck.o objects/opt/pile.o \
	  objects/opt/run.o objects/opt/foundation.o objects/opt/stock.o \
		objects/opt/tableau.o objects/opt/board.o objects/opt/command.o \
		objects/opt/move_cmd.o objects/opt/hint_cmd.o objects/opt/move.o \
		objects/opt/location.o


.PHONY: all clean test tidy profile bz_solitaire bz_dbg_solitaire bz_prof_solitaire test_gtest test_e2e

all: solitaire test_solitaire run

clean:
	rm -f *.o *.a objects/**/*.o solitaire gtest dbg_solitaire prof_solitaire \
		gprof.out gmon.out

run: solitaire
	./solitaire play

test: test_gtest dbg_solitaire solitaire
	./full_solve_tests.sh

tidy:
	clang-format -i bin/*.cpp lib/*.h lib/*.cpp test/*.cpp --style=google


objects/dbg/%.o: lib/%.cpp lib/%.h
	$(CC) -o $@ -c $< $(CC_FLAGS)

objects/prof/%.o: lib/%.cpp lib/%.h
	$(CC) -o $@ -c $< $(CC_FLAGS) -pg

objects/opt/%.o: lib/%.cpp lib/%.h
	$(CC) -o $@ -c $< $(CC_FLAGS) -O3

dbg_solitaire: bin/solitaire.cpp $(objects)
	$(CC) $(objects) -g -lgflags $(CC_FLAGS) -o dbg_solitaire bin/solitaire.cpp

solitaire: bin/solitaire.cpp $(opt_objects)
	$(CC) $(opt_objects) -O3 -lgflags $(CC_FLAGS) -o solitaire bin/solitaire.cpp

bz_solitaire:
	CC=clang++ bazel build --platform_suffix=clang-opt --copt -O3 :solitaire

bz_dbg_solitaire:
	CC=clang++ bazel build -c dbg --platform_suffix=clang-dbg :solitaire

bz_prof_solitaire:
	CC=clang++ bazel build -c dbg --platform_suffix=clang-prof --copt -pg :solitaire

prof_solitaire: bin/solitaire.cpp $(prof_objects)
	$(CC) $(prof_objects) -g -pg -lgflags $(CC_FLAGS) -o prof_solitaire bin/solitaire.cpp

test_gtest: gtest
	./gtest

test_e2e: bz_solitaire bz_dbg_solitaire
	./full_solve_tests.sh

test/gtest_solitaire_test.o: test/gtest_solitaire_test.cpp lib/*.cpp lib/*.h $(GTEST_HEADERS)
	$(CC) -o $@ -c $< $(CC_FLAGS) -lgtest

test/card_test.o: test/card_test.cpp lib/*.cpp lib/*.h $(GTEST_HEADERS)
	$(CC) -o $@ -c $< $(CC_FLAGS)

test/deck_test.o: test/deck_test.cpp lib/*.cpp lib/*.h $(GTEST_HEADERS)
	$(CC) -o $@ -c $< $(CC_FLAGS)

isstuck_benchmark: benchmark/isstuck_benchmark.cpp $(objects)
	$(CC) -v $< $(objects) -lbenchmark $(CC_FLAGS) -o $@ 

gtest: test/card_test.o test/deck_test.o test/gtest_solitaire_test.o gtest_main.a $(objects)
	$(CC) $(CC_FLAGS) -lpthread $^ -o gtest

profile: prof_solitaire
	./prof_solitaire play 4 < inputs/seed4_another_input.txt
	gprof ./prof_solitaire > gprof.out



# From https://github.com/google/googletest/blob/release-1.8.1/googletest/make/Makefile
# With tweaks from me, even though they said you shouldn't need to... maybe WSL's fault?
gtest-all.o : $(GTEST_SRCS_)
		$(CC) $(CC_FLAGS) -I$(GTEST_IDIR) -I$(GTEST_SRCDIR) -c \
			            $(GTEST_SRCDIR)/src/gtest-all.cc

gtest_main.o : $(GTEST_SRCS_)
		$(CC) $(CC_FLAGS) -I$(GTEST_IDIR) -I$(GTEST_SRCDIR) -c \
			            $(GTEST_SRCDIR)/src/gtest_main.cc

gtest.a : gtest-all.o
		$(AR) $(ARFLAGS) $@ $^

gtest_main.a : gtest-all.o gtest_main.o
		$(AR) $(ARFLAGS) $@ $^
