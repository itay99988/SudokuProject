#ifndef MAINAUX_H_
#define MAINAUX_H_

void doSave(Board* board, char *path, int gameMode);
void doSolve(char *path, Board** userBoard, List** undoList,int mode, int currentMarkErrors);
void doEdit(char *path,Board** userBoard, List** undoList, int mode);
void doValidate(Board* board);
void doNumSolutions(Board* board);
void doAutoFill(Board* board, List* undoList);
void doGenerate(Board* userBoard, List* undoList, int x, int y);
void setOptions(Board *board, int row, int column);
void destroyBoard(Board *currentBoard);
Board* copyBoard(Board *currentBoard);
void resetBoard(Board *board);
int isBoardFull(Board *currentBoard);
int isBoardEmpty(Board *currentBoard);


#endif /* MAINAUX_H_ */
