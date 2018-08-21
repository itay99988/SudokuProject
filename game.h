/*
 * game.h
 *
 *  Created on: 13 במאי 2018
 *      Author: Itay Cohen
 */


#ifndef GAME_H_
#define GAME_H_

/* ---The cell struct--
 * this struct is used for describing a specific sudoku cell
 * contains the follwing:
 * -value field
 * -fixed field
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

Cell** init(int size);
void printBoard(Cell **board, int N, int n, int m);
int set(Cell **board, int x, int y, int z, int n, int m);
int hint(Cell **solvedBoard, int x, int y);
int validate(Cell **generatedBoard, Cell **userBoard);
void exitGame(Cell **generatedBoard, Cell **userBoard, int size);
void initUserBoard(Cell** userBoard, int fixedCells, int size);
int restart(Cell **generatedBoard, Cell **userBoard, int n, int m);
int startGame(int n, int m);
#endif /* GAME_H_ */
