#ifndef SOLVER_H_
#define SOLVER_H_

int isValid(Board *board, int row, int column, int value);
int isBoardValid(Board *board);
int detBacktracking(Board* board);
int randBacktracking(Board* board);
void autoFill(Board *board,List *undoList);
int countDetBacktracking(Board* board);
int numSolutions(Board* board);
void markErrors(Board *board, int row, int column, int value);
int isThereAnError(Board *board);
#endif /* SOLVER_H_ */
