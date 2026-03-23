# Game_of_Life
 A simple cellular automaton that my team and i'm implemented called the Game of Life.

In order to get a working game, you need to compile the source code located in the `src` folder.
You need have a `ncurses.h ` library. It is recommended to use a `gcc` compiler for compilation.

Linux 
```
gcc game_of_life.c -o game_of_life.out -std=c11 -Wall -Wextra -Werror -lncurses
```

The initial state of the game can be set in three ways:
1. Pass the word "random" as an argument to the program. This will make the initial state random.
2. Pass the path to the file containing the initial state as an argument to the program.
3. You can also pass the file by redirecting standard input.

By default, the playing field is 80x25 characters.
The '@' symbol corresponds to the alive state of a cell.
You can configure this in the header file and recompile the game.

You can use example files located in the `samples` folder.
They describe the classical initial states of this game.
