# My Very Good Solitaire Project


[![C/C++ CI](https://github.com/kemcbride/my_very_good_klondike/actions/workflows/c-cpp.yml/badge.svg)](https://github.com/kemcbride/my_very_good_klondike/actions/workflows/c-cpp.yml)

## How to build & run

This project uses Make, bazel, and clang++ with C++20 standards.

The project also uses:
- gflags (https://github.com/gflags/gflags/blob/master/INSTALL.md)
- gtest (https://github.com/google/googletest)
- clang-format (https://clang.llvm.org/docs/ClangFormat.html)
- benchmark (https://google.github.io/benchmark/user_guide.html)

You'll need to install bazel, but once you've got that set up, all you need is:

```
bazel run :solitaire play
```

----

## What's this All About

Basically just a little bit of practice writing C++, implementing solitaire!
Technically, I want to implement the game, and then implement a few algorithms
that try to win, and see how good they are on average.

Maybe it'd be cool to have a Microsoft-like tool that can generate games of
varying difficulty...

* Right now you can play (and win) games.
* The game can tell if you're stuck, and can tell if you've won.
* You can ask for hints, like in microsoft solitaire!

There are probably still some bugs floating around though...


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
