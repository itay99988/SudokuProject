/*
 * mainAux.h
 *
 *  Created on: 13 במאי 2018
 *      Author: Yotam
 */

#ifndef MAINAUX_H_
#define MAINAUX_H_

void addOption(int *options, int value);
void printArray(int *arr, int size);
void removeOption(int *options, int index);
void setOptions(Cell **board, int row, int column, int n, int m);
int* copyOption(int *options, int size);
void copyIntoOption(int *fromOptions, int *toOptions, int size);
Cell** copyBoard(Cell **currentBoard, int size);
void copyIntoBoard(Cell **fromBoard, Cell **toBoard, int size);
void destroyBoard(Cell **currentBoard, int size);
void resetBoard(Cell **board,int size);

#endif /* MAINAUX_H_ */
