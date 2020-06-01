# My Very Good Solitaire Project


## How to build & run

This project uses Make and clang++ with C++17 standards (I use std::optional).
You should be able to clone this repo to your computer using `git clone --recurse-submodules`,
and then run `make solitaire` inside the repo's directory to build the binary.

Then, to run the program, just do: `./solitaire` from that same directory!

----

## What's this All About

Basically just a little bit of practice writing C++, implementing solitaire!
Technically, I want to implement the game, and then implement a few algorithms
that try to win, and see how good they are on average.

Maybe it'd be cool to have a Microsoft-like tool that can generate games of
varying difficulty...

* Right now you can play (and win) games.
* The game can tell if you're stuck, and can tell if you've won.
* You can ask for hints, like in microsoft solitaire! The hints are mostly useful!

There are probably still some bugs floating around though...

-----

## TODO

* ~make isSolved() check actually work~
* ~implement "new game" command~
* ~implement "reset" command (just in case...)~
* ~revolutionize the makefile (lol)~
* ~make isStuck() check work~
* ~add hint() command~
* refactor commands? that could clean up solitaire.cpp (ehhhhhhh idk) (or clean up unused command stuff)
* implement undo? (implementing undo stack wouldn't be hard, but implementing the undo itself)
* implement scoring tools
* implement draw-3 mode
* implement game timer?
* integrate with some third party libraries (ncurses?) to practice using third party libraries


-----


A snippet of example output:

```
cmd: n
 f1    f2    f3    f4      s
( - ) ( - ) ( A♠) ( - )   <6♥>
p1 [5♣]
p2 [X][Q♣]
p3 [X][X][10♥]
p4 [X][X][X][Q♦]
p5 [X][X][X][X][9♦]
p6 [X][X][X][X][X][6♣]
p7 [X][X][X][X][X][X][7♣]
cmd: n
 f1    f2    f3    f4      s
( - ) ( - ) ( A♠) ( - )   <A♣>
p1 [5♣]
p2 [X][Q♣]
p3 [X][X][10♥]
p4 [X][X][X][Q♦]
p5 [X][X][X][X][9♦]
p6 [X][X][X][X][X][6♣]
p7 [X][X][X][X][X][X][7♣]
cmd: m s f3
Invalid move: Cannot push A♣ onto ♠foundation
 f1    f2    f3    f4      s
( - ) ( - ) ( A♠) ( - )   <A♣>
p1 [5♣]
p2 [X][Q♣]
p3 [X][X][10♥]
p4 [X][X][X][Q♦]
p5 [X][X][X][X][9♦]
p6 [X][X][X][X][X][6♣]
p7 [X][X][X][X][X][X][7♣]
cmd: m s f4
 f1    f2    f3    f4      s
( - ) ( - ) ( A♠) ( A♣)   <8♦>
p1 [5♣]
p2 [X][Q♣]
p3 [X][X][10♥]
p4 [X][X][X][Q♦]
p5 [X][X][X][X][9♦]
p6 [X][X][X][X][X][6♣]
p7 [X][X][X][X][X][X][7♣]
cmd:
```
