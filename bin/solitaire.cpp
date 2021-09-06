/* solitaire.cpp
 * contains some stuff to define cards and a deck for solitaire play.
 * klondike specfiically, for now.
 * @kemcbride/@ke2mcbri 2020
 */

#include "lib/board.h"
#include "lib/deck.h"
#include "lib/move_cmd.h"

#include <chrono>
#include <iostream>
#include <random>
#include <string>

#include <gflags/gflags.h>

using namespace std;

DEFINE_bool(autoreveal, true, "Newly revealed cards will be flipped for the player automatically");
DEFINE_bool(autosolve, true, "Once all cards are revealed, the game will auto-solve itself for the player");

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
  helpstr += "* r, restart: restart and deal a new game\n";
  helpstr += "* t, toggle: toggle board labels\n";
  helpstr += "* b, board: print the board\n";
  helpstr += "* n, next: go to next card in stock\n";
  helpstr += "* i, hint: print a possible move. cycles through possible moves\n";
  helpstr += "* score: print current score\n";
  helpstr += "* solve: solve the current game, if all cards have been revealed\n";
  helpstr +=
      "* m, move: move <s><#> <d> - attempt to move # cards from s to d\n";
  helpstr +=
      "  from stock or fdn, do not provide a count. from pile, please do.\n";
  helpstr += "  eg. m p2 3 p1; m s f3; m f1 p4; m s p7";
  return helpstr;
}

string get_cmd() {
  cout << "cmd: ";
  string cmd;
  getline(cin, cmd);
  cout << "Your command was: " << cmd << endl;
  return cmd;
}

bool is_not_exit(string str) { return (str != "exit" && str != "x"); }

bool is_help(string str) { return (str == "help" || str == "h"); }

bool is_restart(string str) { return (str == "restart" || str == "r"); }

bool is_toggle(string str) { return (str == "toggle" || str == "t"); }

bool is_next(string str) { return (str == "next" || str == "n"); }

bool is_hint(string str) { return (str == "hint" || str == "i"); }

bool is_score(string str) { return (str == "score"); }

bool is_solve(string str) { return (str == "solve"); }

bool is_move(string str) {
  string first_bit = string(str, 0, str.find(" "));
  return (first_bit == "move" || first_bit == "m");
}

string ignore_first_bit(string str) {
  return string(str, str.find(" ") + 1, str.size());
}

Board new_game() {
  Deck d;
  d.shuffle();
  Board b(d);
  return b;
}

int play() {
  cout << "Welcome to the game! (h=help, x=exit)" << endl;

  Board b = new_game();

  string cmd = get_cmd();
  while (!cin.eof() && is_not_exit(cmd)) {
    if (is_help(cmd)) {
      cout << game_help() << endl;
    } else if (is_restart(cmd)) {
      cout << "Restarting the game." << endl;
      b = new_game();
    } else if (is_toggle(cmd)) {
      b.toggleLabels();
    } else if (is_next(cmd)) {
      b.next();
      cout << b.toString() << endl;
    } else if (is_hint(cmd)) {
      cout << b.hint() << endl;
    } else if (is_score(cmd)) {
      cout << b.getScore() << endl;
    } else if (is_solve(cmd)) {
      auto solveResult = b.trySolve();
      if (solveResult == false) {
        cout << "Board cannot be solved from present state" << endl;
      }
      cout << b.toString() << endl;
    } else if (is_move(cmd)) {
      try {
        MoveCmd m_cmd(ignore_first_bit(cmd));
        b.move(m_cmd);
      } catch (runtime_error &e) {
        cerr << e.what() << endl;
      } catch (invalid_argument &e) {
        cerr << e.what() << " error: invalid argument, no count in your p move?"
             << endl;
      }
      cout << b.toString() << endl;
    } else {
      cout << b.toString() << endl;
    }

    cmd = get_cmd();
  }
  return b.getScore();
}

int main(int argc, char **argv) {
  std::srand(
      std::time(nullptr)); // use current time as seed for random generator
  gflags::SetUsageMessage(program_help());
  gflags::ParseCommandLineFlags(&argc, &argv, true);

  if (argc == 1) {
    cout << program_help() << endl;
  } else if (argc > 1) {

    string cmd = argv[1];
    if (cmd == "help") {
      cout << program_help() << endl;
    } else if (cmd == "play") {
      if (argc >= 3) {
        // if extra arg provided, use as seed. single character only.. -_-
        int seed = char_to_int(argv[2][0]);
        srand(seed);
      }
      return play();
    } else {
      cout << program_help() << endl;
    }
  }
  return 0;
}
