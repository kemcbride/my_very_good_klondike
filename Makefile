CC=clang++
CC_FLAGS=-Wall -g -I. --std=c++17

objects = card.o deck.o pile.o run.o foundation.o stock.o tableau.o board.o \
	  move_cmd.o move.o location.o

.PHONY: all clean test

all: solitaire test_solitaire run

run: solitaire
	./solitaire play

test: test_solitaire
	./test_solitaire

clean:
	rm -f *.o solitaire

%.o: lib/%.cpp lib/%.h
	$(CC) -o $@ -c $< $(CC_FLAGS) 

solitaire: bin/solitaire.cpp $(objects)
	$(CC) -o solitaire  bin/solitaire.cpp $(objects) $(CC_FLAGS)

test_solitaire: test/test_solitaire.cpp Catch2/single_include/catch2/* $(objects)
	$(CC) -o test_solitaire $< $(objects) $(CC_FLAGS)
