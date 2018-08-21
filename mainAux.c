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

#define BOARDSIZE 9
#define BLOCKSIZE 3



/*
 * addOption
 *
 *  This function add a number to the optional values array of a specific cell
 *  @param options - pointer to the array
 *  @param value - the inserted number
 *  @return -
 */
void addOption(int *options, int value)
{
	int i;
	for (i = 0; i<BOARDSIZE;i++)
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
void removeOption(int *options, int index)
{
	int i;
	for (i = index; i<BOARDSIZE-1;i++)
	{
		options[i]=options[i+1];
	}
	options[BOARDSIZE-1]=0;
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
void setOptions(Cell **board, int row, int column, int n, int m)
{
	int k;

	for (k=1;k<=BOARDSIZE; k++)
	{
		if (isValid(board,row, column, k, n, m)==1)
		{
			addOption(board[row][column].options, k);
			board[row][column].numOfOptions++;
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
void destroyBoard(Cell **currentBoard, int size)
{
	int k,l;
	for(k=0;k<size;k++)
	{
		for(l=0;l<size;l++)
		{
			if(currentBoard[k][l].options){
				free(currentBoard[k][l].options);
				currentBoard[k][l].options = NULL;
			}
		}
		if(currentBoard[k])
		{
			free(currentBoard[k]);
		}
	}
	if(currentBoard)
	{
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
Cell** copyBoard(Cell **currentBoard, int size)
{
	Cell **newBoard;
	int i,l;
	int k;
	newBoard = malloc(size*sizeof(Cell *));
	if(!newBoard)
		{
			printf("Error: malloc has failed\n");
			exit(0);
			return NULL;
		}
	for(i=0;i<size;i++)
	{
		newBoard[i] = malloc(sizeof(Cell) *size);
		if(!newBoard[i])
			{
				printf("Error: malloc has failed\n");
				destroyBoard(newBoard,size);
				exit(0);
				return NULL;
			}
	}
	for(k=0;k<size;k++)
	{
		for(l=0;l<size;l++)
		{
			newBoard[k][l].value = currentBoard[k][l].value;
			newBoard[k][l].fixed = currentBoard[k][l].fixed;
			newBoard[k][l].options = copyOption(currentBoard[k][l].options,size);
			newBoard[k][l].numOfOptions = currentBoard[k][l].numOfOptions;
		}
	}

	return newBoard;
}

/*
 * copyBoard
 *
 *  This function completely copies some board to an existing location in memory
 *  @param fromBoard - pointer to copied board
 *  @param toBoard - pointer to new board
 *  @param size - size of board
 *  @return -
 */
void copyIntoBoard(Cell **fromBoard, Cell **toBoard, int size){
	int k,l;

	for(k=0;k<size;k++)
	{
		for(l=0;l<size;l++)
		{
			toBoard[k][l].value = fromBoard[k][l].value;
			toBoard[k][l].fixed = fromBoard[k][l].fixed;
			copyIntoOption(fromBoard[k][l].options,toBoard[k][l].options,size);
			toBoard[k][l].numOfOptions = fromBoard[k][l].numOfOptions;
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
void resetBoard(Cell **board,int size)
{
	int k,l;
		for(k=0;k<size;k++)
		{
			for(l=0;l<size;l++)
			{
				board[k][l].value = 0;
				board[k][l].fixed = 0;
				resetOption(board[k][l].options,size);
				board[k][l].numOfOptions = 0;
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
