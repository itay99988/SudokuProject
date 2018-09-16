#ifndef SOLVER_H_
#define SOLVER_H_

/*
 * Solver Module
 *
 *  This module is in charge of the game's solution. every function which is related to
 *  a specific operation/stage in the solution is here. (such as isValid value for a cell, num_solutions, validate, etc)
 */

#include "game.h"
#include "undoList.h"

/*
 * isValid
 *
 *  This function check if a specific value is valid for a cell
 *  @param board - the game board
 *  @param row - cell's row
 *  @param column - cell's column
 *  @param value - cell's desired value
 *  @return - 0 if the value is not valid, 1 otherwise
 */
int isValid(Board *board, int row, int column, int value);

/*
 * validate
 *
 *  This function gets the actual user board, makes a copy of it and then runs ILP on the copy
 *  in order if the board is solvable.
 *  @param board - the actual game board (not a copy of it)
 *  @return - 1 if solveable, 0 if not.
 */
int validate(Board* board);

/*
 * generate
 *
 *  This function gets an empty board in edit mode, randomly chooses X cells and fills them
 *  with valid values, solves the board and then shows only Y random cells out of N^2 available
 *  cells. tries this 1000 times if it fails.
 *
 *  @param board - the actual game board
 *  @param undoList - pointer to the undo list
 *  @param x - as shown in the description
 *  @param y - as shown in the description
 *  @return -1 if function succeeded, 0 if not.
 */
int generate(Board* userBoard, List *undoList, int x, int y);

/*
 * autoFill
 *
 *  This function gets the gameboard and the undoList, go over each cell and checks if there's only 1 valid value
 *  for it. if yes - sets it (by push it to the stack and go over the stack after), print the action and insert it
 *  to the undoList (again - by go over the stack)
 *
 *  @param board - the actual game board
 *  @param undoList - pointer to the undo list
 *  @return -
 */
void autoFill(Board *board,List *undoList);

/*
 * getNumSolutions
 *
 *  This function gets a game board, and returns the number of the valid solutions for this
 *  board. this time we use an iterative solution to solve the problem. we use stack to demonstrate
 *  the recursive calls. we push and pop from the stack depends on the iterations until we count
 *  all the possible solutions
 *
 *  @param board - the actual game board
 *  @return - number of possible solutions
 */
int getNumSolutions(Board* board);

/*
 * markErrors
 *
 *  This function is called after a cell was change - therefore gets the gameboard and a specific cell -
 *  meaning row and column. it goes over the row, the column and the block and mark error each cell that was affected by
 *  the set action.
 *
 *  @param board - the actual game board
 *  @param row - the cell's row
 *  @param column - the cell's column
 *  @return -
 */
void markErrors(Board* board, int row, int column);

/*
 * markAllBoardErrors
 * REPLACE - check if it is correctly, modified isn't even used
 * REPLACE - document it after
 *
 *  This function gets the gameboard goes over all the cells and update their error value by checking if it is valid
 *
 *  @param board - the actual game board
 *  @return -
 */
void markAllBoardErrors(Board* board);

/*
 * isThereAnError
 *
 *  This function gets the gameboard and check if there's any cell marked with error. it assumes the cells are marked
 *  correctly, meaning that markErrors or markAllBoardErrors was called before
 *
 *  @param board - the actual game board
 *  @return - 1 if there's at least 1 error in the board, 0 otherwise
 */
int isThereAnError(Board *board);
#endif /* SOLVER_H_ */
