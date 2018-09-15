/*
 * MainAux Module
 *
 *  This module is in charge of auxilary functions in the project.
 *  The functions here maniplute data, manage memory and are in charge of printing
 *  every command has a doCommand which validate the data and call the command method (which in the game module)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "game.h"
#include "mainAux.h"
#include "solver.h"
#include "tools.h"
#include "ILPSolver.h"

/* private methods declaration: */
void addOption(int *options, int value, int boardsize);
void removeOption(int *options, int index, int boardsize);
int* copyOption(int *options, int size);
void resetOption(int *options, int size);
void printArray(int *arr, int size);
void markAsFixed(Board *currentBoard);
void clearFixed(Board *currentBoard);
int isInt(char* string);

/* Public methods: */

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
void doSave(Board* userBoard, char *path, int mode)
{
	if (mode==1) /* solve mode */
	{
		save(userBoard, path, mode);
	}
	else /* mode==2 ---> edit mode */
	{
		if(isThereAnError(userBoard))
			printf("Error:board contains erroneous values\n");
		else if(validate(userBoard))
				save(userBoard, path, mode);
			else
				printf("Error: board validation failed\n");
	}
}

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
void doSolve(char *path, Board** userBoard, List** undoList,int* mode, int currentMarkErrors)
{
	FILE* fp;
	fp = fopen(path, "r");
	if (fp==NULL)
	{
		printf("Error: File doesn't exist or cannot be opened\n");
	}
	else
	{
		(*mode) = 1; /* start a puzzle in solve mode */
		destroyBoard(*userBoard);
		load(path,userBoard,*mode);
		destroyList(*undoList);
		(*undoList) = initList();
		(*userBoard)->markErrors = currentMarkErrors;
		printBoard(*userBoard);
		fclose(fp);
	}
}

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
void doEdit(char *path,Board** userBoard, List** undoList, int* mode)
{
	FILE* fp;
	if (path!=NULL) /*check if there is a parameter*/
	{
		fp = fopen(path, "r");
		if (fp==NULL)
		{
			printf("Error: File doesn't exist or cannot be opened\n");
		}
		else
		{
			(*mode) = 2; /* start a puzzle in edit mode */
			destroyBoard(*userBoard);
			load(path,userBoard,*mode);
			(*userBoard)->markErrors = 1;/* mark errors parameter is 1 */
			destroyList(*undoList);
			*undoList = initList();
			printBoard(*userBoard);
			fclose(fp);
		}
	}
	else /* there isn't a parameter - initalize an empty board */
	{
		destroyBoard(*userBoard);
		(*mode) = 2; /* start a puzzle in edit mode */
		/* need to initilalize an empty board */
		*userBoard = init(3,3); /* initiate 3*3 - maybe change it to a DEFINE or something REPLACE*/
		(*userBoard)->markErrors = 1;/* mark errors parameter is 1 */
		destroyList(*undoList);
		*undoList = initList();
		printBoard(*userBoard);
	}
}

/*
 * doValidate
 *
 *  This function validates the board and prints message respectively
 *  @param userBoard - the user's board
 *  @return -
 */
void doValidate(Board* userBoard)
{
	if(isThereAnError(userBoard)) /*check whether there is an erroneous value in the board*/
		printf("Error: board contains erroneous values\n");
	else{
		if(validate(userBoard))
			printf("Validation passed: board is solvable\n");
		else
			printf("Validation failed: board is unsolvable\n");
	}
}

/*
 * doNumSolutions
 *
 *  This function validates whether the board is erroneous or not, if not - gets the number of solutions and prints it
 *  @param userBoard - the user's board
 *  @return -
 */
void doNumSolutions(Board* userBoard){

	int numSolutions;
	if(isThereAnError(userBoard))
		printf("Error: board contains erroneous values\n");
	else
	{
		numSolutions=getNumSolutions(userBoard);
		printf("Number of solutions: %d\n", numSolutions);

		if(numSolutions==1)
			printf("This is a good board!\n");
		else if(numSolutions>1)
			printf("The puzzle has more than 1 solution, try to edit it further\n");
	}
}

/*
 * doAutoFill
 *
 *  This function validates whether the board is erroneous or not, if not - autofill it and check if the board is solved
 *  @param userBoard - the user's board
 *  @param list - the doubly linked list which stores the moves
 *  @param mode - the current game mode
 *  @return -
 */
void doAutoFill(Board* userBoard, List* undoList, int* mode){
	if(isThereAnError(userBoard))
		printf("Error: board contains erroneous values\n");
	else
	{
		autoFill(userBoard,undoList);
		printBoard(userBoard);

		/* autofill may solve the board - therefore need to check it and update the game mode respectively*/
		if(isBoardFull(userBoard))
		{
			if (isThereAnError(userBoard))
				printf("Puzzle solution erroneous\n");
			else
			{
				printf("Puzzle solved successfully\n");
				(*mode) = 0; /*game is solved, move to INIT mode*/
			}
		}
	}
}

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
void doGenerate(Board* userBoard, List* undoList, char* first, char* second){
	int result,x,y,boardsize, numberOfCells;

	x = atoi(first);
	y = atoi(second);
	boardsize = userBoard->boardsize;
	numberOfCells = boardsize*boardsize;

	if (!isInt(first) || !isInt(second)) /* x and y are integers */
		printf("Error: value not in range 0-%d\n",numberOfCells);
	else if (!((x>=0 && x<=numberOfCells) && (y>=0 && y<=numberOfCells))) /* x and y between 0 and number of cells */
		printf("Error: value not in range 0-%d\n",numberOfCells);
	else
	{
		if(!isBoardEmpty(userBoard))
			printf("Error: board is not empty\n");
		else{
			result = generate(userBoard, undoList, x, y);
			if(!result)
				printf("Error: puzzle generator failed\n");
			else
				printBoard(userBoard);
		}
	}
}

/*
 * doHint
 *
 *  This function validates the user's input for hint, and call hint or prints error respectively
 *  @param userBoard - the user's board
 *  @param first - the first field the user sent to the command
 *  @param second - the second field the user sent to the command
 *  @return -
 */
void doHint(Board* userBoard, char* first,char* second){
	int x,y,boardsize, solved;
	Board* fullBoard;
	x = atoi(first);
	y = atoi(second);
	boardsize = userBoard->boardsize;

	if (!isInt(first) || !isInt(second)) /* x and y are integers */
		printf("Error: value not in range 1-%d\n",boardsize);
	else if((x==0 && strcmp(first,"0")!=0)||(y==0 && strcmp(second,"0")!=0)) /* x and y are integers */
			printf("Error: value not in range 1-%d\n",boardsize);
	else if (!((x>=1 && x<=boardsize) && (y>=1 && y<=boardsize))) /* x and y between 1 and number of cells */
		printf("Error: value not in range 1-%d\n",boardsize);
	else if (isThereAnError(userBoard)){
		printf("Error: board contains erroneous values\n");
	}
	else
	{
		if (userBoard->cells[y-1][x-1].fixed==1)
			printf("Error: cell is fixed\n");
		else if (userBoard->cells[y-1][x-1].value!=0)
			printf("Error: cell already contains a value\n");
		else
		{
			/*run ILP and get a solved board*/
			fullBoard = copyBoard(userBoard);
			solved = ilpSolve(fullBoard);
			if (solved==0)
				printf("Error: board is unsolvable\n");
			else
				hint(fullBoard,y-1,x-1);
			/*free the solved board's memory*/
			destroyBoard(fullBoard);
		}
	}

}

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
void doSet(Board* userBoard, List* undoList, char* first, char* second,char* third, int* mode){
	int x,y,z, boardsize, solved;
	x = atoi(first);
	y = atoi(second);
	z = atoi(third);
	boardsize = (userBoard)->boardsize;

	if (!isInt(first) || !isInt(second) || !isInt(third)) /* x,y and z are integers */
		printf("Error: value not in range 0-%d\n",boardsize);
	else if((x==0 && strcmp(first,"0")!=0)||(y==0 && strcmp(second,"0")!=0)||(z==0 && strcmp(third,"0")!=0))
		printf("Error: value not in range 0-%d\n",boardsize);
	else if (!((x>=1 && x<=boardsize) && (y>=1 && y<=boardsize))) /* x and y are between 1 and board size */
		printf("Error: value not in range 1-%d\n",boardsize);
	else if (!(z>=0 && z<=boardsize)) /* z is between 0 and board size */
		printf("Error: value not in range 0-%d\n",boardsize);
	else
	{
		solved = set(userBoard, undoList ,y-1,x-1,z, *mode);

		/* if the board is solved, change to INIT mode */
		if (solved == 2)
		{
			(*mode) = 0;
		}
	}
}

/*
 * doMarkErrors
 *
 *  This function validates the user's input for markErrors, and call markErrors or prints error respectively
 *  @param userBoard - the user's board
 *  @param first - the first field the user sent to the command
 *  @param lastBoardMarkErrors - the last board markErrors value
 *  @return -
 */
void doMarkErrors(Board* userBoard, char* first, int* lastBoardMarkErrors){
	int value;
	value = atoi(first);

	if(isInt(first) && value>=0 && value<=1) /* field can be only 0 or 1 */
	{
		userBoard ->markErrors = atoi(first);
		(*lastBoardMarkErrors) = atoi(first);
	}
	else
		printf("Error: the value should be 0 or 1\n");
}

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
void doUndo(Board* board, List* undoList, int printVal, int* mode){
	undo(board,undoList,printVal);

	if (*mode==1)  /*relevant only to solve mode */
	{
		if(isBoardFull(board))
		{
			if (isThereAnError(board))
				printf("Puzzle solution erroneous\n");
			else
			{
				printf("Puzzle solved successfully\n");
				(*mode) = 0;
			}
		}
	}

}
/*
 * setOptions
 *
 *  This function sets the cell's options for values
 *  @param board - the game board
 *  @param row - cell's row
 *  @param column - cell's column
 *  @return -
 */
void setOptions(Board *board, int row, int column)
{

	int k,boardsize;
	boardsize=board->boardsize;
	for (k=1;k<=boardsize; k++)
	{
		if (isValid(board,row, column, k)==1)
		{
			addOption(board->cells[row][column].options, k, boardsize);
			board->cells[row][column].numOfOptions++;
		}
	}
}


/*
 * destroyBoard
 *
 *  This function completely frees memory of an existing board
 *  @param currentBoard - pointer to board
 *  @param size - size of board
 *  @return -
 */
void destroyBoard(Board *currentBoard)
{
	int k,l,size;
	if(currentBoard){
		size = currentBoard->boardsize;
		for(k=0;k<size;k++)
		{
			for(l=0;l<size;l++)
			{
				if(currentBoard->cells[k][l].options){
					free(currentBoard->cells[k][l].options);
					currentBoard->cells[k][l].options = NULL;
				}
			}
			if(currentBoard->cells[k])
			{
				free(currentBoard->cells[k]);
			}
		}
		if(currentBoard->cells)
			free(currentBoard->cells);
		free(currentBoard);
	}
}
/*
 * copyBoard
 *
 *  This function completely copies existing board to a new location in memory
 *  @param currentBoard - pointer to board
 *  @return - pointer to the new board
 */
Board* copyBoard(Board *currentBoard)
{
	Board *wholeBoard;
	Cell **newBoard;
	int i,l;
	int k,n,m,size;

	size = currentBoard->boardsize, n=currentBoard->n, m=currentBoard->m;
	wholeBoard = malloc(sizeof(Board));
	if(!wholeBoard)
	{
		printf("Error: malloc has failed\n");
		exit(0);
		return NULL;
	}
	newBoard = malloc(size*sizeof(Cell *));
	if(!newBoard)
	{
		printf("Error: malloc has failed\n");
		exit(0);
		return NULL;
	}

	/* the cells creation */
	for(i=0;i<size;i++)
	{
		newBoard[i] = malloc(sizeof(Cell) *size);
		if(!newBoard[i])
			{
				printf("Error: malloc has failed\n");
				exit(0);
				return NULL;
			}
	}
	for(k=0;k<size;k++)
	{
		for(l=0;l<size;l++)
		{
			newBoard[k][l].value = currentBoard->cells[k][l].value;
			newBoard[k][l].fixed = currentBoard->cells[k][l].fixed;
			newBoard[k][l].error = currentBoard->cells[k][l].error;
			newBoard[k][l].options = copyOption(currentBoard->cells[k][l].options,size);
			newBoard[k][l].numOfOptions = currentBoard->cells[k][l].numOfOptions;
		}
	}

	/* the board creation */
	wholeBoard->boardsize = size;
	wholeBoard->cells = newBoard;
	wholeBoard->n = n;
	wholeBoard->m = m;

	return wholeBoard;
}

/*
 * resetBoard
 *
 *  This function resets all values of a exisiting board to its default values.
 *  @param board - pointer to board
 *  @return -
 */
void resetBoard(Board *board)
{
	int k,l,size;
	size = board->boardsize;

	for(k=0;k<size;k++)
	{
		for(l=0;l<size;l++)
		{
			board->cells[k][l].value = 0;
			board->cells[k][l].fixed = 0;
			board->cells[k][l].error = 0;
			resetOption(board->cells[k][l].options,size);
			board->cells[k][l].numOfOptions = 0;
		}
	}
}

/*
 * isBoardFull
 *
 *  This function check whether all cells in the board contains value
 *  @param currentBoard - pointer to board
 *  @return 1 - all cells contains values (not 0!!!), 0 - otherwise
 */
int isBoardFull(Board *currentBoard)
{
	int i, j;
	int size = currentBoard->boardsize;

	for (i =0; i<size; i++)
		for (j =0; j<size; j++)
			if(currentBoard->cells[i][j].value==0)
				return 0;

	return 1;
}

/*
 * isBoardEmpty
 *
 *  This function check whether all cells in the board do not contains value
 *  @param currentBoard - pointer to board
 *  @return 1 - board is empty, 0 - otherwise
 */
int isBoardEmpty(Board *currentBoard)
{
	int i, j;
	int size = currentBoard->boardsize;

	for (i =0; i<size; i++)
		for (j =0; j<size; j++)
			if(currentBoard->cells[i][j].value!=0)
				return 0;

	return 1;
}

/* End of public methods */

/* Private methods: */

/*
 * copyOption
 *
 *  This function copies an options array to a new location in memory (inc. allocation)
 *  @param options - pointer to the options array
 *  @param size - size of array
 *  @return - the new array's pointer
 */
int* copyOption(int *options, int size)
{
	int k;
	int* newOptions = calloc(size,sizeof(int));
	if(!newOptions)
	{
		printf("Error: calloc has failed\n");
		exit(0);
		return NULL;
	}
	for(k=0;k<size;k++){
		newOptions[k] = options[k];
	}

	return newOptions;
}

/*
 * resetOption
 *
 *  This function resets an options array
 *  @param fromOptions - pointer to the copied options array
 *  @param size - size of array
 *  @return -
 */
void resetOption(int *options, int size)
{
	int k;
	for(k=0;k<size;k++){
		options[k] = 0;
	}
}

/* not in use right now. did you want to use it? REPLACE */
void markAsFixed(Board *currentBoard){
	int i, j;
	int size = currentBoard->boardsize;

	for (i =0; i<size; i++)
		for (j =0; j<size; j++)
			if(currentBoard->cells[i][j].value!=0)
				currentBoard->cells[i][j].fixed=1;
}

/* not in use right now. did you want to use it? REPLACE*/
void clearFixed(Board *currentBoard){
	int i, j;
	int size = currentBoard->boardsize;

	for (i =0; i<size; i++)
		for (j =0; j<size; j++)
				currentBoard->cells[i][j].fixed=0;
}

/*
 * printArray
 *
 *  This function prints an int array - for maintenance only
 *  @param board - pointer to the ints array
 *  @param size - size of array
 *  @return -
 */
void printArray(int *arr, int size)
{
	int i;
	for (i = 0;i<size;i++)
	{
		printf("%d ", arr[i]);
	}

	printf("\n");
}

/*
 * addOption
 *
 *  This function add a number to the optional values array of a specific cell
 *  @param options - pointer to the array
 *  @param value - the inserted number
 *  @return -
 */
void addOption(int *options, int value, int boardsize)
{
	int i;
	for (i = 0; i<boardsize;i++)
	{
		if (options[i]==0)
		{
			options[i]=value;
			break;
		}
	}
}

/*
 * removeOption
 *
 *  This function removes a number from the optional values array of a specific cell
 *  @param options - pointer to the array
 *  @param index - the index of the removed value
 *  @return -
 */
void removeOption(int *options, int index, int boardsize)
{
	int i;
	for (i = index; i<boardsize-1;i++)
	{
		options[i]=options[i+1];
	}
	options[boardsize-1]=0;
}

/* End of private methods */

/* Private methods: */

/*
 * isInt
 *
 *  This function gets a string and check whether all his chars are integers
 *  @param string - a string
 *  @return 1 - all chars are integers, 0 - otherwise
 */
int isInt(char *string)
{
        int i, stringLength = strlen(string);

        for(i = 0; i < stringLength; i++)
        {
                if(isdigit(string[i]) == 0) /*|| ispunct(string[i]) != 0 ... */
                        break;
        }

        if(i != stringLength)
                return 0;
        else
                return 1;
}
