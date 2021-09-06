CC=clang++
CC_FLAGS=-Wall -g -I. --std=c++17
CPUS ?= $(shell (nproc --all || sysctl -n hw.ncpu) 2>/dev/null || echo 1)
MAKEFLAGS += --jobs=$(CPUS)

objects = card.o deck.o pile.o run.o foundation.o stock.o tableau.o board.o \
	  command.o move_cmd.o hint_cmd.o move.o location.o

.PHONY: all clean test

all: solitaire test_solitaire run

run: solitaire
	./solitaire play

test: test_solitaire solitaire
	./test_solitaire
	./full_solve_tests.sh

clean:
	rm -f *.o solitaire

%.o: lib/%.cpp lib/%.h
	$(CC) -o $@ -c $< $(CC_FLAGS) 

solitaire: bin/solitaire.cpp $(objects)
	$(CC) -o solitaire  bin/solitaire.cpp $(objects) $(CC_FLAGS) -lgflags

test_solitaire: test/test_solitaire.cpp Catch2/single_include/catch2/* $(objects)
	$(CC) -o test_solitaire $< $(objects) $(CC_FLAGS)
