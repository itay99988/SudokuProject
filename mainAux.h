#ifndef MAINAUX_H_
#define MAINAUX_H_

void addOption(int *options, int value, int boardsize);
void removeOption(int *options, int index, int boardsize);
void setOptions(Board *board, int row, int column);
int* copyOption(int *options, int size);
void copyIntoOption(int *fromOptions, int *toOptions, int size);
void resetOption(int *options, int size);
void destroyBoard(Board *currentBoard);
Board* copyBoard(Board *currentBoard);
void copyIntoBoard(Board *fromBoard, Board *toBoard);
void resetBoard(Board *board);
void printArray(int *arr, int size);

#endif /* MAINAUX_H_ */
