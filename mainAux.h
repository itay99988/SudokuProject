#ifndef MAINAUX_H_
#define MAINAUX_H_
/*
 * MainAux Module
 *
 *  This module is in charge of auxilary functions in the project.
 *  The functions here maniplute data, manage memory and are in charge of printing
 *  every command has a doCommand which validate the data and call the command method (which in the game module)
 */

/*
 * doSave
 *
 *  This function call the save method if we are in solve mode, otherwise, we are in edit mode and we validate the board
 *  if validated - being saved
 *  @param userBoard - the user's board
 *  @param path - a pointer to the desired path
 *  @param mode - the current game mode
 *  @return -
 */
void doSave(Board* board, char *path, int gameMode);


/*
 * doSolve
 *
 *  This function validate the path it gets, and init the board from the file respectively
 *  @param path - a pointer to the desired path
 *  @param userBoard - the user's board
 *  @param list - the doubly linked list which stores the moves
 *  @param mode - the current game mode
 *  @param currentMarkErrors - the current mark errors field
 *  @return -
 */
void doSolve(char *path, Board** userBoard, List** undoList,int* mode, int currentMarkErrors);

/*
 * doEdit
 *
 *  This function validate the path it gets, and init the board from the file respectively
 *  @param path - a pointer to the desired path
 *  @param userBoard - the user's board
 *  @param list - the doubly linked list which stores the moves
 *  @param mode - the current game mode
 *  @return -
 */
void doEdit(char *path,Board** userBoard, List** undoList, int* mode);

/*
 * doValidate
 *
 *  This function validates the board and prints message respectively
 *  @param userBoard - the user's board
 *  @return -
 */
void doValidate(Board* board);

/*
 * doNumSolutions
 *
 *  This function validates whether the board is erroneous or not, if not - gets the number of solutions and prints it
 *  @param userBoard - the user's board
 *  @return -
 */
void doNumSolutions(Board* board);

/*
 * doAutoFill
 *
 *  This function validates whether the board is erroneous or not, if not - autofill it and check if the board is solved
 *  @param userBoard - the user's board
 *  @param list - the doubly linked list which stores the moves
 *  @param mode - the current game mode
 *  @return -
 */
void doAutoFill(Board* board, List* undoList, int* mode);

/*
 * doGenerate
 *
 *  This function validates the user's input for generate, and call generate or prints error respectively
 *  @param userBoard - the user's board
 *  @param list - the doubly linked list which stores the moves
 *  @param first - the first field the user sent to the command
 *  @param second - the second field the user sent to the command
 *  @return -
 */
void doGenerate(Board* userBoard, List* undoList, char* first, char* second);

/*
 * doHint
 *
 *  This function validates the user's input for hint, and call hint or prints error respectively
 *  @param userBoard - the user's board
 *  @param first - the first field the user sent to the command
 *  @param second - the second field the user sent to the command
 *  @return -
 */
void doHint(Board* userBoard, char* first,char* second);

/*
 * doMarkErrors
 *
 *  This function validates the user's input for markErrors, and call markErrors or prints error respectively
 *  @param userBoard - the user's board
 *  @param first - the first field the user sent to the command
 *  @param lastBoardMarkErrors - the last board markErrors value
 *  @return -
 */
void doMarkErrors(Board* userBoard, char* first, int* lastBoardMarkErrors);

/*
 * doSet
 *
 *  This function validates the user's input for set, and call set or prints error respectively
 *  @param userBoard - the user's board
 *  @param list - the doubly linked list which stores the moves
 *  @param first - the first field the user sent to the command
 *  @param second - the second field the user sent to the command
 *  @param third - the third field the user sent to the command
 *  @param mode- the current game mode
 *  @return -
 */
void doSet(Board* userBoard, List* undoList, char* first,char* second, char* third, int* mode);

/*
 * doUndo
 *
 *  This function call undo method, and prints respective message if the board solutions erroneous
 *  @param userBoard - the user's board
 *  @param list - the doubly linked list which stores the moves
 *  @param printVal - whether print or not the undone moves
 *  @param mode - the current game mode
 *  @return -
 */
void doUndo(Board* board, List* undoList, int printVal, int* mode);

/*
 * setOptions
 *
 *  This function sets the cell's options for values
 *  @param board - the game board
 *  @param row - cell's row
 *  @param column - cell's column
 *  @return -
 */
void setOptions(Board *board, int row, int column);

/*
 * destroyBoard
 *
 *  This function completely frees memory of an existing board
 *  @param currentBoard - pointer to board
 *  @param size - size of board
 *  @return -
 */
void destroyBoard(Board *currentBoard);

/*
 * copyBoard
 *
 *  This function completely copies existing board to a new location in memory
 *  @param currentBoard - pointer to board
 *  @return - pointer to the new board
 */
Board* copyBoard(Board *currentBoard);

/*
 * resetBoard
 *
 *  This function resets all values of a exisiting board to its default values.
 *  @param board - pointer to board
 *  @return -
 */
void resetBoard(Board *board);

/*
 * isBoardFull
 *
 *  This function check whether all cells in the board contains value
 *  @param currentBoard - pointer to board
 *  @return 1 - all cells contains values (not 0!!!), 0 - otherwise
 */
int isBoardFull(Board *currentBoard);

/*
 * isBoardEmpty
 *
 *  This function check whether all cells in the board do not contains value
 *  @param currentBoard - pointer to board
 *  @return 1 - board is empty, 0 - otherwise
 */
int isBoardEmpty(Board *currentBoard);


#endif /* MAINAUX_H_ */
