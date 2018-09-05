#ifndef SOLVER_H_
#define SOLVER_H_

#include "game.h"
#include "undoList.h"

int isValid(Board *board, int row, int column, int value);
int validate(Board* board);
int generate(Board* userBoard, List *undoList, int x, int y);
void autoFill(Board *board,List *undoList);
int countDetBacktracking(Board* board);
int getNumSolutions(Board* board);
void markErrors(Board* board, int row, int column);
void markAllBoardErrors(Board* board);
int isThereAnError(Board *board);
#endif /* SOLVER_H_ */
