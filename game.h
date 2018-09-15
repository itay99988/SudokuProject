#ifndef GAME_H_
#define GAME_H_

/*
 * Game Module
 *
 *  This module is in charge of game-play. every function which is related to
 *  a specific operation/stage in the game is here. (such as initialize board, set,hint,validate etc)
 */

#include "undoList.h"

/* ---The cell struct--
 * this struct is used for describing a specific sudoku cell
 * contains the following:
 * -value field
 * -fixed field
 * -error value
 * -optional values array
 * -number of optional values in array field
 */
typedef struct cell{
	int value;
	int fixed;
	int error;
	int *options;
	int numOfOptions;
} Cell;

/* ---The board struct--
 * this struct is used for describing a sudoku board
 * contains the follwing:
 * -game cells
 * -n,m parameters
 * -the boardsize parameter (n*m)
 * markErrors field (1=mark, 0=do not mark)
 */
typedef struct board{
	Cell **cells;
	int n;
	int m;
	int boardsize;
	int markErrors;
} Board;

/*
 * init
 *
 *  This function initializes a board with default values
 *  @param n,m - n,m fields of the board
 *  @return - Board* - the initiate Board
 */
Board* init(int n, int m);

/*
 * printBoard
 *
 *  This function prints the current status of the sudoku board, according to the desired format
 *  0 counts as empty cell, dot addition represents fixed cell, asterisk addition represents erroneous cell
 *  @param board - the solved board
 *  @return -
 */
void printBoard(Board *board);

/*
 * set
 *
 *  This function gets partially solved board, and sets a value in the desired location, if possible
 *  @param board - the user's board
 *  @param list - the doubly linked list which stores the moves
 *  @param x - column number
 *  @param y - row number
 *  @param z - value
 *  @param gameMode - the current mode of the game
 *  @return - 0 if set has failed. 1 if set has succeeded. 2 if puzzle is solved successfully.
 */
int set(Board *board, List *undoList, int x, int y, int z, int gameMode);

/*
 * hint
 *
 *  This function gets solved board, and prints a hint to the user for a specific location (based on the stored solution)
 *  @param solvedBoard - a solve board, according to the user's board
 *  @param x - column number
 *  @param y - row number
 *  @return -
 */
void hint(Board *solvedBoard, int x, int y);

/*
 * redo
 *
 *  This function redoes the last undone operation of user that affected the board.
 *
 *  @param board - pointer to the game board
 *  @param undoList - the doubly linked list which stores the moves
 *  @param printVal - whether of not the function print its actions
 *  @param mode - a pointer to the current game mode
 *  @return -
 */
void redo(Board* board, List* undoList, int printVal, int* mode);

/*
 * undo
 *
 *  This function undoes the last operation of user that affected the board.
 *
 *  @param board - pointer to the game board
 *  @param undoList - the doubly linked list which stores the moves
 *  @param printVal - whether of not the function print its actions
 *  @return -
 */
void undo(Board* board, List* undoList, int printVal);

/*
 * reset
 *
 *  This function reset the game board to his first stage, meaning undo all the moves that were done by the user
 *  @param userBoard - the user's board
 *  @param undoList - a pointer to the doubly linked list which stores the moves
 *  @return -
 */
void reset(Board* board, List** undoList);

/*
 * exitGame
 *
 *  This function free all memory block and quits.
 *  @param userBoard - the user's board
 *  @param undoList - the doubly linked list which stores the moves
 *  @return -
 */
void exitGame(Board *userBoard, List* undoList);

/*
 * startGame
 *
 *  This function will initialize the sudoku game for the first time
 *  prints the desired "line" and call the read function
 *  @return -
 */
void startGame();
#endif /* GAME_H_ */
