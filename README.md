# My Very Good Solitaire Project

Basically just a little bit of practice writing C++, implementing solitaire!
Technically, I want to implement the game, and then implement a few algorithms
that try to win, and see how good they are on average.

Maybe it'd be cool to have a Microsoft-like tool that can generate games of
varying difficulty...

Anyways, not getting ahead of myself. All it does now is shuffle decks
and print cards.


------

Unfortunately terminal colors not preserved... but if you can imagine, red cards are printed in bold red font!

```
ke2mcbri:solitaire./solitaire
Shuffling and drawing top card a few times...
5♣
7♣
4♠
7♥
9♦
6♠
7♦
2♠
6♥
2♥
Now let's draw a few hands.
2♥ 5♦ 3♠ K♥ 9♥
10♣ 6♥ 8♣ Q♣ 6♣
8♦ A♣ 10♦ 6♦ 2♠
```


-----

Update: You can now move cards from the stock to a foundation!

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
