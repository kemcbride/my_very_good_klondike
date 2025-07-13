/* solitaire.cpp
 * contains some stuff to define cards and a deck for solitaire play.
 * klondike specfiically, for now.
 * @kemcbride/@ke2mcbri 2020
 * blahblahblah
 */

#include <gflags/gflags.h>

#include <chrono>
#include <iostream>
#include <random>
#include <ranges>
#include <sstream>
#include <string>

#include "lib/board.h"
#include "lib/deck.h"
#include "lib/move_cmd.h"

using namespace std;

DEFINE_bool(
    autoreveal, true,
    "Newly revealed cards will be flipped for the player automatically");
DEFINE_bool(autosolve, true,
            "Once all cards are revealed, the game will auto-solve itself for "
            "the player");
DEFINE_bool(recycle_penalty_enabled, true,
            "-100 pts for each full cycle through the stock");

vector<string> game_log;

const string program_help() {
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
  helpstr += "* l, log: print out all commands you've given so far\n";
  helpstr += "* r, restart: restart and re-deal the game\n";
  helpstr += "* g, newgame: deal a new game\n";
  helpstr += "* t, toggle: toggle board labels\n";
  helpstr += "* b, board: print the board\n";
  helpstr += "* n, next: go to next card in stock\n";
  helpstr +=
      "* i, hint: print a possible move. cycles through possible moves\n";
  helpstr += "* score: print current score\n";
  helpstr +=
      "* solve: solve the current game, if all cards have been revealed\n";
  helpstr +=
      "* m, move: move <s><#> <d> - attempt to move # cards from s to d\n";
  helpstr +=
      "  from stock or fdn, do not provide a count. from pile, please do.\n";
  helpstr += "  eg. m p2 3 p1; m s f3; m f1 p4; m s p7";
  return helpstr;
}

bool is_not_exit(string str) {
  return (str != "exit" && str != "x" && str != "quit" && str != "q");
}

bool is_help(string str) { return (str == "help" || str == "h"); }

bool is_gamelog(string str) { return (str == "log" || str == "l"); }

bool is_restart(string str) { return (str == "restart" || str == "r"); }

bool is_newgame(string str) { return (str == "newgame" || str == "g"); }

bool is_toggle(string str) { return (str == "toggle" || str == "t"); }

bool is_next(string str) { return (str == "next" || str == "n"); }

bool is_hint(string str) { return (str == "hint" || str == "i"); }

bool is_score(string str) { return (str == "score"); }

bool is_solve(string str) { return (str == "solve"); }

bool is_move(string str) {
  string first_bit = string(str, 0, str.find(" "));
  return (first_bit == "move" || first_bit == "m");
}

int play(mt19937 generator) {
  cout << "Welcome to the game! (h=help, x=exit, b=show board)" << endl;

  Deck d(generator);
  d.shuffle();
  Board b = Board(d, FLAGS_autosolve, FLAGS_autoreveal,
                  FLAGS_recycle_penalty_enabled);

  cout << "cmd: ";
  string cmd;
  cin >> cmd;
  while (!cin.eof() && is_not_exit(cmd)) {
    if (is_help(cmd)) {
      cout << game_help() << endl;
    } else if (is_gamelog(cmd)) {
      cout << "Game log thus far:\n";
      for (auto line : game_log) {
        cout << line << "\n";
      }
    } else if (is_restart(cmd)) {
      // Note - the intent here is to preserve the last shuffle, so you can
      // replay the game you just played.
      cout << "Restarting the game." << endl;
      d.reset_index();
      b = Board(d, FLAGS_autosolve, FLAGS_autoreveal,
                FLAGS_recycle_penalty_enabled);
    } else if (is_newgame(cmd)) {
      cout << "Dealing a new game" << endl;
      d.shuffle();
      d.reset_index();
      b = Board(d, FLAGS_autosolve, FLAGS_autoreveal,
                FLAGS_recycle_penalty_enabled);
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
        MoveCmd m_cmd = MoveCmd::create();
        b.move(m_cmd);
        cmd = cmd + " " + m_cmd.toString();
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

    game_log.push_back(cmd);
    cout << "cmd: ";
    cin >> cmd;
  }
  return b.getScore();
}

int main(int argc, char **argv) {
  std::random_device rd;
  std::mt19937 g(rd());
  // gflags::SetUsageMessage(program_help());
  gflags::ParseCommandLineFlags(&argc, &argv, true);

  if (argc == 1) {
    cout << program_help() << endl;
  } else if (argc > 1) {
    string cmd = argv[1];
    if (cmd == "help") {
      cout << program_help() << endl;
    } else if (cmd == "play") {
      if (argc >= 3) {
        int seed;
        istringstream ss(argv[2]);
        if (!(ss >> seed)) {
          cerr << "Invalid seed: " << argv[2] << endl;
        } else if (!ss.eof()) {
          cerr << "Trailing characters after seed: " << argv[2] << endl;
        }
        g = std::mt19937(seed);
      }
      play(g);
    } else {
      cout << program_help() << endl;
    }
  }
  return 0;
}
