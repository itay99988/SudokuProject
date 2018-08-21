/*
 * solver.h
 *
 *  Created on: 13 במאי 2018
 *      Author: Yotam
 */

#ifndef SOLVER_H_
#define SOLVER_H_

int detBacktracking(int size,Cell** board);
int isValid(Cell **board, int row, int column, int value, int m, int n);
int isBoardValid(Cell **board, int N);
void autoFill(Cell **board, int m, int n);
int randBacktracking(int size,Cell** board);

#endif /* SOLVER_H_ */
