#ifndef SOLVER_H_
#define SOLVER_H_

int isValid(Board *board, int row, int column, int value);
int isBoardValid(Board *board);
int detBacktracking(Board* board);
int randBacktracking(Board* board);
void autoFill(Board *board);
#endif /* SOLVER_H_ */
