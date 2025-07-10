CC=clang++
CXX=clang++
CC_FLAGS=-Wall -Wextra -pthread -g -I. --std=c++20 -stdlib=libc++
# On osx using homebrew, may need to add to CC_FLAGS:
# -I$(shell (brew --prefix))/include -L$(shell (brew --prefix))/lib

GFLAGS_IDIR = /usr/include/gflags
GTEST_IDIR = /usr/include/gtest
GTEST_SRCDIR = /usr/src/gtest
GTEST_HEADERS = $(GTEST_IDIR)/*.h \
				$(GTEST_IDIR)/internal/*.h
GTEST_SRCS_ = $(GTEST_SRCDIR)/src/*.cc $(GTEST_SRCDIR)/src/*.h $(GTEST_HEADERS)

CPUS ?= $(shell (nproc --all || sysctl -n hw.ncpu) 2>/dev/null || echo 1)
MAKEFLAGS += --jobs=$(CPUS)

objects = card.o deck.o pile.o run.o foundation.o stock.o tableau.o board.o \
	  command.o move_cmd.o hint_cmd.o move.o location.o

.PHONY: all clean test

all: solitaire test_solitaire run

run: solitaire
	./solitaire play

test: test_gtest solitaire
	./full_solve_tests.sh

clean:
	rm -f *.o *.a solitaire

%.o: lib/%.cpp lib/%.h
	$(CC) -o $@ -c $< $(CC_FLAGS)

solitaire: bin/solitaire.cpp $(objects)
	$(CC) $(objects) -lgflags $(CC_FLAGS) -o solitaire bin/solitaire.cpp

test_gtest: gtest
	./gtest

test/gtest_solitaire_test.o: test/gtest_solitaire_test.cpp lib/*.cpp lib/*.h $(GTEST_HEADERS)
	$(CC) -o $@ -c $< $(CC_FLAGS) -lgtest

test/card_test.o: test/card_test.cpp lib/*.cpp lib/*.h $(GTEST_HEADERS)
	$(CC) -o $@ -c $< $(CC_FLAGS) -lgtest

gtest: test/card_test.o test/gtest_solitaire_test.o gtest_main.a $(objects)
	$(CC) $(CC_FLAGS) -lpthread $^ -o gtest



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
