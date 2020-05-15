CC=g++
CC_FLAGS=-Wall -g -I. --std=c++17

objects = card.o deck.o pile.o run.o foundation.o stock.o tableau.o board.o move.o

.PHONY: all clean

all: solitaire

clean:
	rm *.o solitaire

%.o: %.cpp %.h
	$(CC) -o $@ -c $< $(CC_FLAGS) 

solitaire: solitaire.cpp $(objects)
	$(CC) -o solitaire  solitaire.cpp $(objects) $(CC_FLAGS)
