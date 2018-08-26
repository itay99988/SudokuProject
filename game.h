
#ifndef GAME_H_
#define GAME_H_

#include "undoList.h"

/* ---The cell struct--
 * this struct is used for describing a specific sudoku cell
 * contains the follwing:
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
 */
typedef struct board{
	Cell **cells;
	int n;
	int m;
	int boardsize;
	int markErrors;
} Board;

Board* init(int n, int m);
void initUserBoard(Board* userBoard, int fixedCells);
void printBoard(Board *board);
int set(Board *board, int x, int y, int z);
int hint(Board *solvedBoard, int x, int y);
int validate(Board *generatedBoard, Board *userBoard);
void redo(Board* board, List* undoList);
void undo(Board* board, List* undoList);
int restart(Board *generatedBoard, Board *userBoard);
void exitGame(Board *generatedBoard, Board *userBoard);
int startGame(int n, int m);
#endif /* GAME_H_ */
