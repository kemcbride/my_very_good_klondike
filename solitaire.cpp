/* solitaire.cpp 
 * contains some stuff to define cards and a deck for solitaire play.
 * klondike specfiically, for now.
 * @kemcbride/@ke2mcbri 2020
 */
#include <iostream>
#include <string>

#include "card.h"
#include "deck.h"
#include "board.h"

using namespace std;

string program_help() {
  string helpstr;
  helpstr += "command help for ./solitaire:\n";
  helpstr += "* ./solitaire      - print this help message\n";
  helpstr += "* ./solitaire help - print this help message\n";
  helpstr += "* ./solitaire play - play the solitaire game!";
  return helpstr;
}

string game_help() {
  string helpstr;
  helpstr += "game help for solitaire:\n";
  helpstr += "* x, exit: quit the game\n";
  helpstr += "* h, help: print this help\n";
  return helpstr;
}

string get_cmd() {
  cout << "cmd: ";
  string cmd;
  cin >> cmd;
  return cmd;
}

bool is_not_exit(string str) {
  return (str != "exit" && str != "x");
}

bool is_help(string str) {
  return (str == "help" || str == "h");
}

int play() {
  cout << "Welcome to the game! (h=help, x=exit)" << endl;

  int score = 0;
  Deck d;
  Board b(d);

  string cmd = get_cmd();
  while (is_not_exit(cmd)) {
    if (is_help(cmd)) {
      cout << game_help() << endl;
    } else {
      cout << b.toString() << endl;
    }

    cmd = get_cmd();
  }
  return score;
}

int main(int argc, char** argv) {
  if (argc == 1) {
    cout << program_help() << endl;
  } else if (argc > 1) {

    string cmd = argv[1];
     if (cmd == "help") {
        cout << program_help() << endl;
    } else if (cmd == "play") {
      return play();
    } else {
      cout << program_help() << endl;
    }

  }
	return 0;
}
