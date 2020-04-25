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
  helpstr += "* t, toggle: toggle board labels\n";
  helpstr += "* m, move: move <s><#> <d> - attempt to move # cards from s to d";
  return helpstr;
}

string get_cmd() {
  cout << "cmd: ";
  string cmd;
  getline(cin, cmd);
  return cmd;
}

bool is_not_exit(string str) {
  return (str != "exit" && str != "x");
}

bool is_help(string str) {
  return (str == "help" || str == "h");
}

bool is_toggle(string str) {
  return (str == "toggle" || str == "t");
}

bool is_next(string str) {
  return (str == "next" || str == "n");
}

bool is_move(string str) {
  string first_bit = string(str, 0, str.find(" "));
  return (first_bit == "move" || first_bit == "m");
}

int play() {
  cout << "Welcome to the game! (h=help, x=exit)" << endl;

  int score = 0;
  Deck d;
  d.shuffle();
  Board b(d);

  string cmd = get_cmd();
  while (is_not_exit(cmd)) {
    if (is_help(cmd)) {
      cout << game_help() << endl;
    } else if (is_toggle(cmd)) {
      b.toggle_labels();
    } else if (is_next(cmd)) {
      b.next();
      cout << b.toString() << endl;
    } else if (is_move(cmd)) {
      cout << "OMG A MVOE MOVE MOVE!!!" << endl;
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
