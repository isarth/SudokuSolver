/**
C file for a program that will solve a given sudoku.
Isabelle Arthur
*/

#include <stdio.h>
#include <stdlib.h>

#include "SudokuSolver.h"

/// Solved grid
Grid solved_grid;

/// Initialize the grid based on user input.
/// @param input array of strings representing each row
/// @param cages list of cages, NULL if none
/// @param num_cages number of cages, 0 if none
/// @return Grid stuct
Grid init_grid(char* input[9], Cage cages[81], int num_cages) {
    Grid grid = (Grid) malloc(sizeof(struct grid_s));
    for (int i = 0; i < 9; i++) {
    	for (int j = 0; j < 9; j++) {
	    grid->g[i][j] = ((int) input[i][j]) - 48;
	}
    }
    for (int i = 0; i < num_cages; i++) {
	grid->c[i] = cages[i];
    }
    grid->num_c = num_cages;
    return grid;
}

/// Check if the grid is full.
/// @param grid sudoku grid
/// @return an integer 0 or 1 (not full or full)
int check_grid(Grid grid) {
    for (int i = 0; i < 9; i++) {
    	for (int j = 0; j < 9; j++) {
	    if (grid->g[i][j] == 0) {
		return 0;
	    }
	}
    }
    return 1;
}

/// Copy a sudoku grid into a new sudoku grid.
/// @param old_grid sudoku grid to copy
/// @return new copied Grid structure
Grid copy_grid(Grid old_grid) {
    Grid grid = (Grid) malloc(sizeof(struct grid_s));
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
	    grid->g[i][j] = old_grid->g[i][j];
	}
    }
    return grid;
}

/// Check if the given cell conflicts with its row, column, or box.
/// @param grid sudoku grid
/// @param row cell row
/// @param col cell column
/// @return an integer 0 or 1 (invalid or valid)
int check_cell(Grid grid, int row, int col) {
    //printf("check_cell\n");
    for (int i = 0; i < 9; i++) {
	if (i != col && grid->g[row][i] == grid->g[row][col]) {
	    return 0;
	}
	if (i != row && grid->g[i][col] == grid->g[row][col]) {
	    return 0;
	}
	if (row <= 2 && col <= 2) {
	    int tr = (int) (i / 3);
            int tc = (i % 3);
            if ((tr != row || tc != col) && grid->g[tr][tc] == 
                grid->g[row][col]) {
                return 0;
            }
	} else if (row <= 2 && col <= 5) {
	    int tr = (int) (i / 3);
            int tc = 3 + (i % 3);
            if ((tr != row || tc != col) && grid->g[tr][tc] == 
                grid->g[row][col]) {
                return 0;
            }
	} else if (row <= 2) {
	    int tr = (int) (i / 3);
            int tc = 6 + (i % 3);
            if ((tr != row || tc != col) && grid->g[tr][tc] == 
                grid->g[row][col]) {
                return 0;
            }
	} else if (row <= 5 && col <= 2) {
	    int tr = 3 + (int) (i / 3);
            int tc = (i % 3);
            if ((tr != row || tc != col) && grid->g[tr][tc] == 
                grid->g[row][col]) {
                return 0;
            }
	} else if (row <= 5 && col <= 5) {
	    int tr = 3 + (int) (i / 3);
	    int tc = 3 + (i % 3);
	    if ((tr != row || tc != col) && grid->g[tr][tc] == 
	    	grid->g[row][col]) {
		return 0;
	    }
	} else if (row <= 5) {
	    int tr = 3 + (int) (i / 3);
            int tc = 6 + (i % 3);
            if ((tr != row || tc != col) && grid->g[tr][tc] == 
                grid->g[row][col]) {
                return 0;
            }
	} else if (col <= 2) {
	    int tr = 6 + (int) (i / 3);
            int tc = (i % 3);
            if ((tr != row || tc != col) && grid->g[tr][tc] == 
                grid->g[row][col]) {
                return 0;
            }
	} else if (col <= 5) {
	    int tr = 6 + (int) (i / 3);
            int tc = 3 + (i % 3);
            if ((tr != row || tc != col) && grid->g[tr][tc] == 
                grid->g[row][col]) {
                return 0;
            }
	} else {
	    int tr = 6 + (int) (i / 3);
            int tc = 6 + (i % 3);
            if ((tr != row || tc != col) && grid->g[tr][tc] == 
                grid->g[row][col]) {
                return 0;
            }
	}
    }
    int b_loop = 0;
    int same = 0;
    if (grid->num_c != 0) {
	//printf("cage detected\n");
	for (int i = 0; i < grid->num_c; i++) {
	    int total = 0;
	    int full = 1;
	    for (int j = 0; j < grid->c[i]->num_cells; j++) {
		//printf("j: %d\n", j);
		int c_r = grid->c[i]->cells[j][0];
		int c_c = grid->c[i]->cells[j][1];
		if (c_r == row && c_c == col) {
		    b_loop = 1;
		} else if (grid->g[c_r][c_c] == grid->g[row][col]) {
		    same = 1;
		}
		if (grid->g[c_r][c_c] == 0) {
		    full = 0;
		}
		total += grid->g[c_r][c_c];
	    }
	    if (b_loop) {
		printf("loop escaped\n");
		if (same) {
		    return 0;
		}
		if (full) {
		    if (grid->c[i]->total == 0 || total == grid->c[i]->total) {
			return 1;
		    }
		    return 0;
		} else if (total < grid->c[i]->total) {
		    return 1;
		}
		return 0;
	    } else {
		same = 0;
	    }
	}
	printf("loop escaped\n");
    }
    return 1;
}

/// Main iterative solving algorithm.
/// @param grid sudoku grid
/// @return sudoku grid
Grid solve_I(Grid grid) {
    Grid old_grid = copy_grid(grid);
    int back_one = 0;
    while (!check_grid(grid)) {
        int row, col, start, cur;
    	int b_loop = 0;
    	for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
            	if (grid->g[i][j] <= 0 && !back_one) {
                    row = i;
                    col = j;
		    start = 0;
                    b_loop = 1;
                    break;
            	} else if (grid->g[i][j] <= 0) {
		    start = cur;
		    b_loop = 1;
		    break;
		} else if (back_one && old_grid->g[i][j] == 0) {
		    cur = grid->g[i][j];
                    row = i;
                    col = j;
		}
            }
            if (b_loop) {
		break;
            }
    	}
        for (int i = start+1; i <= 10; i++) {
	    if (i > 9) {
		back_one = 1;
		grid->g[row][col] = 0;
		break;
	    }
	    grid->g[row][col] = i;
	    if (!check_cell(grid, row, col)) {
		if (i == 9) {
		    back_one = 1;
		    grid->g[row][col] = 0;
		    break;
		}
		continue;
	    }
	    back_one = 0;
	    break;
	}
    }
    free_grid(old_grid);
    return grid;
}

/// Main recursive solving algorithm.
/// @param grid current sudoku grid
/// @return an integer 0 or 1 (invalid or valid)
int solve_R(Grid grid) {
    int row, col;
    int b_loop = 0;
    for (int i = 0; i < 9; i++) {
    	for (int j = 0; j < 9; j++) {
	    if (grid->g[i][j] <= 0) {
		row = i;
		col = j;
		b_loop = 1;
		break;
	    }
	}
	if (b_loop) {
	    break;
	}
    }
    for (int i = 1; i < 10; i++) {
	grid->g[row][col] = i;
	printf("row: %d, col: %d, num: %d\n", row, col, i);
    	if (!check_cell(grid, row, col)) {
	    if (i == 9) {
		grid->g[row][col] = 0;
		return 0;
	    }
	    continue;
	}
	if (check_grid(grid)) {
	    solved_grid = copy_grid(grid);
	    free_grid(grid);
	    return 1;
	}
	if (solve_R(grid)) {
	    return 1;
	}
    }
    grid->g[row][col] = 0;
    return 0;
}

/// Start to solve the given sudoku grid.
/// @param grid sudoku grid
/// @return solved sudoku grid
Grid solve_sudoku(Grid grid) {
    solve_R(grid);
    return solved_grid;
}

/// Free given Grid structure.
/// @param grid Grid structure
void free_grid(Grid grid) {
    free(grid);
}

/// Print the passed sudoku grid in a user-friendly way.
/// @param grid sudoku grid
void print_grid(Grid grid) {
    for (int i = 0; i < 9; i++) {
	if (i % 3 == 0 && i != 0) {
	    printf("----------------------\n");
	}
	for (int j = 0; j < 9; j++) {
	    if (j % 3 == 0 && j != 0) {
	    	printf("| ");
		if (grid->g[i][j] > 0) {
		    printf("%d ", grid->g[i][j]);
		} else {
		    printf(". ");
		}
	    } else {
	    	if (grid->g[i][j] > 0) {
		    printf("%d ", grid->g[i][j]);
		} else {
		    printf(". ");
		}
	    }
	}
	printf("\n");
    }
}
