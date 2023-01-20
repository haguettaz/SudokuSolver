# SudokuSolver

SudokoSolver is a program to play and solve sudokus using an iterative constraint propagation algorithm.

## How to Play?

The program presents two modes: **playing** and **solving**. 

In playing mode, one can navigate on the grid and add digits to it. The program automatically checks whether no constraint (row, column or block) is violated. One can load three prescribed grids by pressing `E` (easy), `M` (multiple solutions), or `H` (hard). The grid can be completely cleaned by pressing `ESCAPE`. 

The solving mode is activated by pressing `SPACE`. Then one can run the solver by pressing `SPACE` again. The initial fixed cells are indicated in black. The deduced ones are in red. To go back to playing mode, press `ESCAPE`.

When multiple solutions have been found, navigate through the different ones using `LEFT` and `RIGHT`.
