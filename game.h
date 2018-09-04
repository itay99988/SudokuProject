
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
void printBoard(Board *board);
int set(Board *board, List *undoList, int x, int y, int z, int gameMode);
void hint(Board *solvedBoard, int x, int y);
void redo(Board* board, List* undoList, int printVal);
void undo(Board* board, List* undoList, int printVal);
void reset(Board* board, List** undoList);
void exitGame(Board *userBoard, List* undoList);
void doSave(Board* board, char *path, int gameMode);
void doSolve(char *path, Board** userBoard, List** undoList,int mode, int currentMarkErrors);
void doEdit(char *path,Board** userBoard, List** undoList, int mode);
void doValidate(Board* board);
void doNumSolutions(Board* board);
void doAutoFill(Board* board, List* undoList);
int validate(Board* board);
void startGame();
#endif /* GAME_H_ */
