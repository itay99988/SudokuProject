/*
 * Solver Module
 *
 *  This module is in charge of the game's solution. every function which is related to
 *  a specific operation/stage in the solution is here. (such as isValid value for a cell, deterministic BackTracking , randomized backTracking , etc)
 */

#include <stdio.h>
#include <stdlib.h>
#include "SPBufferset.h"
#include <string.h>
#include <time.h>
#include "game.h"
#include "mainAux.h"

#define BOARDSIZE 9
#define BLOCKSIZE 3


/*
 * isValid
 *
 *  This function check if a specific value is valid for a cell
 *  @param board - the game board
 *  @param row - cell's row
 *  @param column - cell's column
 *  @param value - cell's desired value
 *  @return - 0 if the value is not valid, 1 otherwise
 */
int isValid(Cell **board, int row, int column, int value)
{
    int i;
    int j;

    int modifiedRow = (row/BLOCKSIZE)*BLOCKSIZE;
    int modifiedColumn = (column/BLOCKSIZE)*BLOCKSIZE;


    for (i=0;i<BOARDSIZE; i++)
    {
    	if (board[row][i].value==value || board[i][column].value==value)
            return 0;
    }

    for (i=0;i<BLOCKSIZE; i++)
        for (j=0;j<BLOCKSIZE; j++)
        {
        	if (board[modifiedRow+i][modifiedColumn+j].value==value)
                return 0;
        }

    return 1;
}


/*
 * isBoardValid
 *
 *  This function check if the all the cells are filled - because the board is finished iff all the cells
 *  were filled.
 *  @param board - the game board
 *  @return - 1 if board is valid, 0 otherwise
 */
int isBoardValid(Cell **board)
{
    int i;
    int j;

    for (i=0;i<BOARDSIZE; i++)
    {
    	for (j=0;j<BOARDSIZE; j++)
		{
			if(board[i][j].fixed==0 && board[i][j].value==0)
				return 0;
		}
    }

    return 1;
}

/*
 * detBacktracking
 *
 *  This function solves the board by deterministic backtracking
 *  @param size - board's size
 *  @param board - the game board
 *  @return -1 if succeeded, 0 if it's not possible
 */
int detBacktracking(int size,Cell** board){
	int i,j,k;

	for(i=0;i<size;i++)
	{
		for(j=0;j<size;j++)
		{
			if(board[i][j].fixed==0 && board[i][j].value==0)
			{
				for (k=1;k<=size;k++)
				{
					if (isValid(board,i, j, k)==1)
					{
						board[i][j].value = k;

						if (detBacktracking(size,board))
							return 1;
					}
				}

				/*
				if (k==10 && board[i][j].value!=9)
				{
					board[i][j].value=0;
					return 0;
				}
				*/
				board[i][j].value=0;
				return 0;
			}

		}
	}

	return 1;
}

/*
 * randBacktracking
 *
 *  This function solves the board by randomized backtracking
 *  @param size - board's size
 *  @param board - the game board
 *  @return -1 if succeeded, 0 if it's not possible
 */
int randBacktracking(int size,Cell** board){
	int i,j,k;
	int randomIndex;
	int originalNumOfOptions;

	for(i=0;i<size;i++)
	{
		for(j=0;j<size;j++)
		{
			if(board[i][j].fixed==0 && board[i][j].value==0)
			{
				setOptions(board,i,j);
				originalNumOfOptions = board[i][j].numOfOptions;
				for (k=0; k<originalNumOfOptions;k++)
				{
					if (board[i][j].numOfOptions==1)
						randomIndex = 0;
					else
						randomIndex = rand()%board[i][j].numOfOptions;

					board[i][j].value = board[i][j].options[randomIndex];

					if (randBacktracking(size,board))
						return 1;


					board[i][j].numOfOptions--;
					removeOption(board[i][j].options,randomIndex);
				}

				board[i][j].value=0;
				return 0;

			}
		}
	}

	return 1;
}




