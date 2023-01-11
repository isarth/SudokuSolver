/**
Main c file to implement and use the SudokuSolver c files.
Isabelle Arthur
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "SudokuSolver.h"

/// Read the given file into a sudoku grid.
/// @param fp given file
/// @return Grid structure
Grid file_to_grid(FILE *fp) {
    printf("file_to_grid\n");
    char* input[10];
    for (int i = 0; i < 9; i++) {
    	char* temp = (char*) malloc(sizeof(10));
	input[i] = temp;
    }
    char line[128];
    int i = 0;
    while (fgets(line, 128, fp) != NULL && i < 9) {
	strcpy(input[i], line);
	printf("%s\n", line);
	i++;
    }
    Cage cage_list[81];
    i = 0;
    while (fgets(line, 128, fp) != NULL) {
	printf("fgets loop\n");
	char* token;
	token = strtok(line, " ");
	printf("%s\n", token);
	if (strcmp(token, "c") == 0) {
	    printf("past token if\n");
	    Cage temp_cage = (Cage) malloc(sizeof(struct cage_s));
	    token = strtok(NULL, " ");
	    char** buf = NULL;
	    temp_cage->total = (int) strtol(token, buf, 10);
	    int row = 0, col = 0;
	    printf("%s\n", token);
	    token = strtok(NULL, " ");
	    while (token != NULL && token[0] != '\0' && token[0] != '\n') {
	        temp_cage->cells[row][col] = (int) strtol(token, buf, 10);
		printf("%d\n", temp_cage->cells[row][col]);
		if (col == 1) {
		    row++;
		    col = 0;
		} else {
		    col = 1;
		}
		token = strtok(NULL, " ");
	    }
	    printf("out of loop\n");
	    temp_cage->num_cells = row;
	    printf("num_cells set to %d\n", row);
	    cage_list[i] = temp_cage;
	    printf("cage added");
	    printf("%d %d\n", temp_cage->cells[0][0], temp_cage->cells[0][1]);
	}
	i++;
    }
    Grid grid = init_grid(input, cage_list, i);;
    for (int i = 0; i < 9; i++) {
	free(input[i]);
    }
    return grid;
}

/// Read in a sudoku grid from a text file and solve it.
/// @param argc number of arguments
/// @param argv arguments
/// @return EXIT_SUCCESS on success and EXIT_FAILURE on failure
int main(int argc, char** argv) {
    // Check if file argument is passed.
    if (argc < 2) {
	fprintf(stderr, "File Needed.\n");
	exit(EXIT_FAILURE);
    }
    // Check if file exists and open it.
    FILE *fp;
    if ((fp = fopen(argv[1], "r")) == NULL) {
	fprintf(stderr, "File Does Not Exist.\n");
	exit(EXIT_FAILURE);
    }
    // Otherwise assume a valid 9x9 grid and initialize it.
    Grid grid = file_to_grid(fp);
    // Close file.
    fclose(fp);
    // Solve sudoku.
    Grid solved = solve_sudoku(grid);
    // Print grid.
    print_grid(solved);
    // Free grid.
    //free(solved->c);
    free_grid(solved);
    // Exit successfully.
    exit(EXIT_SUCCESS);
}
