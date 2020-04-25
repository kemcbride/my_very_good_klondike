CC=g++
CC_FLAGS=-Wall -g -I. --std=c++17

solitaire: solitaire.cpp card.o deck.o pile.o run.o foundation.o stock.o tableau.o board.o move.o
	$(CC) -o solitaire  solitaire.cpp card.o deck.o pile.o run.o foundation.o stock.o tableau.o board.o move.o $(CC_FLAGS)

card.o: card.cpp card.h
	$(CC) -c -o card.o card.cpp $(CC_FLAGS)

deck.o: deck.cpp deck.h
	$(CC) -c -o deck.o deck.cpp $(CC_FLAGS)

pile.o: pile.cpp pile.h
	$(CC) -c -o pile.o pile.cpp $(CC_FLAGS)

run.o: run.cpp run.h
	$(CC) -c -o run.o run.cpp $(CC_FLAGS)

foundation.o: foundation.cpp foundation.h
	$(CC) -c -o foundation.o foundation.cpp $(CC_FLAGS)

stock.o: stock.cpp stock.h
	$(CC) -c -o stock.o stock.cpp $(CC_FLAGS)

tableau.o: tableau.cpp tableau.h
	$(CC) -c -o tableau.o tableau.cpp $(CC_FLAGS)

board.o: board.cpp board.h
	$(CC) -c -o board.o board.cpp $(CC_FLAGS)

move.o: move.cpp move.h
	$(CC) -c -o move.o move.cpp $(CC_FLAGS)
