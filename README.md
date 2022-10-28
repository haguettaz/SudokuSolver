# SudokuSolver

SudokoSolver is a program to play and solve sudokus using an iterative constraint propagation algorithm.

It seems to be able to solve all *normal* sudokus, i.e., sudokus intended to be solved within a reasonable time and without computer help.

## How to Play?

The program presents two modes: **playing** and **solving**. 

In playing mode, one can navigate on the grid and add digits to it. The program automatically checks whether no constraint (row, column or block) is violated. One can load three prescribed grids by pressing `E` (easy), `M` (medium), or `H` (hard). Note that the hard grid is currently not solvable by the program as it requires a guessing strategy at some point. The grid can be completely cleaned by pressing `ESCAPE`. 

The solving mode is activated by pressing `RETURN`. Then one can run the solver one step at a time using `SPACE` or solve it in one shot with `RETURN`. The initial fixed cells are indicated in black. The deduced ones are in blue. At each iteration, the impossible values are removed from the grid. To go back to playing mode, press `ESCAPE`.
