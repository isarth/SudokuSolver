/**
Header file for a c program that will solve a given sudoku.
Isabelle Arthur
*/

#ifndef SUDOKU_SOLVER_H
#define SUDOKU_SOLVER_H

/// Killer cages structure.
struct cage_s {
    int total;
    int num_cells;
    int cells[81][2];
};

typedef struct cage_s * Cage;

/// Sudoku grid structure.
struct grid_s {
    int g[9][9];
    Cage c[81];
    int num_c;
};

typedef struct grid_s * Grid;

/// Initialize the grid based on user input.
/// @param input array of strings representing each row
/// @param cages list of cages, NULL if none
/// @param num_cages number of cages, 0 if none
/// @return Grid stuct
Grid init_grid(char** input, Cage cages[81], int num_cages);

/// Start to recursively solve the given sudoku grid.
/// @param grid sudoku grid
/// @return solved sudoku grid
Grid solve_sudoku(Grid grid);

/// Free given Grid structure.
/// @param grid Grid structure
void free_grid(Grid grid);

/// Print the passed sudoku grid in a user-friendly way.
/// @param grid sudoku grid
void print_grid(Grid grid);

#endif
