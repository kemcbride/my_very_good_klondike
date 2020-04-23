CC=g++
CC_FLAGS=-Wall -g -I. --std=c++17

solitaire: solitaire.cpp card.o deck.o board.o
	$(CC) -o solitaire  solitaire.cpp card.o deck.o board.o $(CC_FLAGS)

card.o: card.cpp card.h
	$(CC) -c -o card.o card.cpp $(CC_FLAGS)

deck.o: deck.cpp deck.h
	$(CC) -c -o deck.o deck.cpp $(CC_FLAGS)

board.o: board.cpp board.h
	$(CC) -c -o board.o board.cpp $(CC_FLAGS)
