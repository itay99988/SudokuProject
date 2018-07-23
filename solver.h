/*
 * solver.h
 *
 *  Created on: 13 במאי 2018
 *      Author: Yotam
 */

#ifndef SOLVER_H_
#define SOLVER_H_

int detBacktracking(int size,Cell** board);
int isValid(Cell **board, int row, int column, int value);
int isBoardValid(Cell **board);
int randBacktracking(int size,Cell** board);

#endif /* SOLVER_H_ */
