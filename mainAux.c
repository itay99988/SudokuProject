/*
 * MainAux Module
 *
 *  This module is in charge of auxilary functions in the project.
 *  The functions here maniplute data, manage memory and are in charge of printing
 */

#include <stdio.h>
#include <stdlib.h>
#include "SPBufferset.h"
#include "game.h"
#include "solver.h"
#include <time.h>

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
 * copyIntoOption
 *
 *  This function copies an options array to a an existing location in memory
 *  @param fromOptions - pointer to the copied options array
 *  @param toOptions - pointer to the new options array
 *  @param size - size of array
 *  @return -
 */
void copyIntoOption(int *fromOptions, int *toOptions, int size)
{
	int k;
	for(k=0;k<size;k++){
		toOptions[k] = fromOptions[k];
	}
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
 * copyIntoBoard
 *
 *  This function completely copies some board to an existing location in memory
 *  @param fromBoard - pointer to copied board
 *  @param toBoard - pointer to new board
 *  @param size - size of board
 *  @return -
 */
void copyIntoBoard(Board *fromBoard, Board *toBoard)
{
	int k,l,size;

	size = fromBoard->boardsize;

	/* copy the basic props first */
	toBoard->boardsize = size;
	toBoard->n = fromBoard->n;
	toBoard->m = fromBoard->m;

	/* now copy the actual cells */
	for(k=0;k<size;k++)
	{
		for(l=0;l<size;l++)
		{
			toBoard->cells[k][l].value = fromBoard->cells[k][l].value;
			toBoard->cells[k][l].fixed = fromBoard->cells[k][l].fixed;
			toBoard->cells[k][l].error = fromBoard->cells[k][l].error;
			copyIntoOption(fromBoard->cells[k][l].options,toBoard->cells[k][l].options,size);
			toBoard->cells[k][l].numOfOptions = fromBoard->cells[k][l].numOfOptions;
		}
	}
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

void markAsFixed(Board *currentBoard){
	int i, j;
	int size = currentBoard->boardsize;

	for (i =0; i<size; i++)
		for (j =0; j<size; j++)
			if(currentBoard->cells[i][j].value!=0)
				currentBoard->cells[i][j].fixed=1;
}

void clearFixed(Board *currentBoard){
	int i, j;
	int size = currentBoard->boardsize;

	for (i =0; i<size; i++)
		for (j =0; j<size; j++)
				currentBoard->cells[i][j].fixed=0;
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
