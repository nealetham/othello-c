# Othello-C

This mini-project implements the game of othello using the C++ programming language. I started this project as tribute to one of my favorite childhood board games, and as a way to familiarize myself with the C++ programming language.

I am doing this in tandem with my parallel computing module at the National University of Singapore, and have hopes to utilize the knowledge learnt in that module and apply it to this project.

## Running the Game

Run `make` to compile the code.
Run `./othello` to start the game!

## Game Tutorial

Currently, the game only supports two player games. All valid plays that can be made by the current player are denoted by an asterisk (\*) mark. To input the position of the next play, the player should type the row number and column number, separated by a white space. For example, `2 4` indicates a play at row 2 and column 4.

The game ends when there are no moves left for both players, or when the board is completely filled.

Have fun!

## Roadmap

- [x] Basic functionality
- [ ] Refactor code
- [ ] Enhance UI
- [ ] Parallelize with OMP
