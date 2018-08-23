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
int isValid(Board *board, int row, int column, int value)
{
    int i,j;
    int n,m,boardsize;
    int modifiedRow, modifiedColumn;

    /* dimensions definition: */
	n=board->n;
	m=board->m;
	boardsize=board->boardsize;

	modifiedRow = (row/m)*m;
	modifiedColumn = (column/n)*n;
    for (i=0;i<boardsize; i++)
    {
    	if (board->cells[row][i].value==value || board->cells[i][column].value==value)
            return 0;
    }

    for (i=0;i<m; i++)
        for (j=0;j<n; j++)
        {
        	if (board->cells[modifiedRow+i][modifiedColumn+j].value==value)
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
int isBoardValid(Board *board)
{
    int i,j,N;

    N=board->boardsize;
    for (i=0;i<N; i++)
    {
    	for (j=0;j<N; j++)
		{
			if(board->cells[i][j].fixed==0 && board->cells[i][j].value==0)
				return 0;
		}
    }

    return 1;
}

/*
 * detBacktracking
 *
 *  This function solves the board by deterministic backtracking
 *  @param board - the game board
 *  @return -1 if succeeded, 0 if it's not possible
 */
int detBacktracking(Board* board)
{
	int i,j,k;
	int size;

	/* dimensions definition: */
	size=board->boardsize;

	for(i=0;i<size;i++)
	{
		for(j=0;j<size;j++)
		{
			if(board->cells[i][j].fixed==0 && board->cells[i][j].value==0)
			{
				for (k=1;k<=size;k++)
				{
					if (isValid(board,i, j, k)==1)
					{
						board->cells[i][j].value = k;

						if (detBacktracking(board))
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
				board->cells[i][j].value=0;
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
int randBacktracking(Board* board)
{
	int i,j,k;
	int size;
	int randomIndex;
	int originalNumOfOptions;

	/* dimensions definition: */
	size=board->boardsize;

	for(i=0;i<size;i++)
	{
		for(j=0;j<size;j++)
		{
			if(board->cells[i][j].fixed==0 && board->cells[i][j].value==0)
			{
				setOptions(board,i,j);
				originalNumOfOptions = board->cells[i][j].numOfOptions;
				for (k=0; k<originalNumOfOptions;k++)
				{
					if (board->cells[i][j].numOfOptions==1)
						randomIndex = 0;
					else
						randomIndex = rand()%board->cells[i][j].numOfOptions;

					board->cells[i][j].value = board->cells[i][j].options[randomIndex];

					if (randBacktracking(board))
						return 1;


					board->cells[i][j].numOfOptions--;
					removeOption(board->cells[i][j].options,randomIndex,size);
				}

				board->cells[i][j].value=0;
				return 0;

			}
		}
	}

	return 1;
}


void autoFill(Board *board)
{
	int i,j;
	int N;
	int optionalValue;
	int theOption = 0;

	/* dimensions definition: */
	N=board->boardsize;

	for (i=0;i<N; i++)
		for (j=0; j<N; j++)
		{
			for (optionalValue = 1; optionalValue <= N; optionalValue++)
				if (isValid(board, i, j, optionalValue)){
					if (theOption == 0){
						theOption = optionalValue;
					}
					else
					{
						theOption = 0;
						break;
					}
				}

			if (theOption != 0)
				board->cells[i][j].value = theOption;
		}
}
