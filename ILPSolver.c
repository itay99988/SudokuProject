/*
 * ILP Solver Module (**Demo**)
 *
 *  This module is a ^^^DEMO^^^ Module for solving a sudoku board. This module uses
 *  backtracking for solving the board. In eclipse, the real ILP Module doesn't pass
 *  compilation, so here we are going to use this demo module. The real one will be used once we
 *  work on the nova server
*/

#include "game.h"

int ilpSolve(Board *userBoard)
{
	int i,j,k;
	int size;

	/* dimensions definition: */
	size=userBoard->boardsize;

	for(i=0;i<size;i++)
	{
		for(j=0;j<size;j++)
		{
			if(userBoard->cells[i][j].fixed==0 && userBoard->cells[i][j].value==0)
			{
				for (k=1;k<=size;k++)
				{
					if (isValid(userBoard,i, j, k)==1)
					{
						userBoard->cells[i][j].value = k;

						if (detBacktracking(userBoard))
							return 1;
					}
				}

				userBoard->cells[i][j].value=0;
				return 0;
			}

		}
	}
	return 1;
}
