/*
 * MainAux Module
 *
 *  This module is in charge of auxilary functions in the project.
 *  The functions here maniplute data, manage memory and are in charge of printing
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

/* Public methods: */

void doSave(Board* userBoard, char *path, int mode)
{
	if (mode==1)
	{
		save(userBoard, path, mode);
	}
	else /*mode==2*/
	{
		if(isThereAnError(userBoard))
			printf("Error:board contains erroneous values\n");
		else if(validate(userBoard))
				save(userBoard, path, mode);
			else
				printf("Error: board validation failed\n");
			/*markAsFixed(userBoard); implement I - check if this is the acctual need*/
	}
}


void doSolve(char *path, Board** userBoard, List** undoList,int mode, int currentMarkErrors)
{
	FILE* fp;
	fp = fopen(path, "r");
	if (fp==NULL)
	{
		printf("Error: File doesn't exist or cannot be opened\n");
	}
	else
	{
		destroyBoard(*userBoard);
		load(path,userBoard,mode);
		destroyList(*undoList);
		(*undoList) = initList();
		(*userBoard)->markErrors = currentMarkErrors;
		printBoard(*userBoard);
	}

	fclose(fp);

}

void doEdit(char *path,Board** userBoard, List** undoList, int mode)
{
	FILE* fp;
	if (path!=NULL) /*there is a parameter*/
	{
		fp = fopen(path, "r");
		if (fp==NULL)
		{
			printf("Error: File doesn't exist or cannot be opened\n");
		}
		else
		{
			destroyBoard(*userBoard);
			load(path,userBoard,mode);

			(*userBoard)->markErrors = 1;/* mark errors parameter is 1 */
			destroyList(*undoList);
			*undoList = initList();
			printBoard(*userBoard);
		}
		fclose(fp);
	}
	else
	{
		destroyBoard(*userBoard);
		/* need to initilalize an empty board */
		*userBoard = init(3,3); /* initiate 3*3 - maybe change it to a DEFINE or something */
		(*userBoard)->markErrors = 1;/* mark errors parameter is 1 */
		destroyList(*undoList);
		*undoList = initList();
		printBoard(*userBoard);
	}
}

void doValidate(Board* userBoard)
{
	if(isThereAnError(userBoard))
		printf("Error: board contains erroneous values\n");
	else{
		if(validate(userBoard))
			printf("Validation passed: board is solvable\n");
		else
			printf("Validation failed: board is unsolvable\n");
	}
}

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

void doAutoFill(Board* userBoard, List* undoList){
	if(isThereAnError(userBoard))
		printf("Error: board contains erroneous values\n");
	else
	{
		autoFill(userBoard,undoList);
		printBoard(userBoard);
	}
}

void doGenerate(Board* userBoard, List* undoList, char* first, char* second){
	int result,x,y,boardsize;

	x = atoi(first);
	y = atoi(second);
	/* need to update empty cells according to d */
	boardsize = userBoard->boardsize; /*do we need it??/*/

	/* need to check - d!!! */
	if (!((x>=0 && x<=boardsize*boardsize) && (y>=0 && y<=boardsize*boardsize))) /*maybe need to change boardsize*/
		printf("Error: value not in range 0-%d\n",boardsize*boardsize); /*fixxxxx this!!!! */
	/* ------------------ */
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

void doHint(Board* userBoard, char* first,char* second){
	int x,y,boardsize, solved;
	Board* fullBoard;
	x = atoi(first);
	y = atoi(second);
	boardsize = userBoard->boardsize; /*do we need it??/*/

	if((x==0 && strcmp(first,"0")!=0)||(y==0 && strcmp(second,"0")!=0))
			printf("Error: value not in range 1-%d\n",boardsize);
	else if (!((x>=1 && x<=boardsize) && (y>=1 && y<=boardsize)))
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
			/*run ILP and get...*/
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

int doSet(Board* userBoard, List* undoList, char* first, char* second,char* third, int mode){
	int x,y,z, boardsize, solved;
	x = atoi(first);
	y = atoi(second);
	z = atoi(third);
	boardsize = (userBoard)->boardsize;

	if((x==0 && strcmp(first,"0")!=0)||(y==0 && strcmp(second,"0")!=0)||(z==0 && strcmp(third,"0")!=0))
			printf("Error: value not in range 0-%d\n",boardsize);
	else if (!((x>=1 && x<=boardsize) && (y>=1 && y<=boardsize) && (z>=0 && z<=boardsize)))
		printf("Error: value not in range 0-%d\n",boardsize);
	else
	{
		solved = set(userBoard, undoList ,y-1,x-1,z, mode);

		return solved;
	}

	return 0;
}

void doMarkErrors(Board* userBoard, char* first){
	/* implement mark errors*/
	if (strcmp(first,"1")==0 || strcmp(first,"0")==0)
	{
		userBoard ->markErrors = atoi(first);
	}
	else
		printf("Error: the value should be 0 or 1\n");
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
 *  @param size - size of board
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
 *  @param size - size of board
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

/* not in use right now. did you want to use it? */
void markAsFixed(Board *currentBoard){
	int i, j;
	int size = currentBoard->boardsize;

	for (i =0; i<size; i++)
		for (j =0; j<size; j++)
			if(currentBoard->cells[i][j].value!=0)
				currentBoard->cells[i][j].fixed=1;
}

/* not in use right now. did you want to use it? */
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

/* End of private methods: */
