/*
 * Game Module
 *
 *  This module is in charge of game-play. every function which is related to
 *  a specific operation/stage in the game is here. (such as initialize board, set,hint,validate etc)
 */

#include <stdio.h>
#include <stdlib.h>
#include "SPBufferset.h"
#include <string.h>
#include <time.h>
#include "game.h"
#include "solver.h"
#include "mainAux.h"
#include "parser.h"

/*
 * init
 *
 *  This function initializes a board with default values
 *  @param size - size of board
 *  @return -
 */
Board* init(int n, int m){

	Cell **board;
	int i;
	int k,l;
	int size;
	Board* newBoard; /*the new board*/
	size=n*m;

	/*first of all, we will create the actual cells of the board*/
	board = malloc(size*sizeof(Cell *));
	if(!board)
	{
		printf("Error: malloc has failed\n");
		exit(0);
		return NULL;
	}
	for(i=0;i<size;i++)
	{
		board[i] = malloc(sizeof(Cell) *size);
		if(!board[i])
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
			board[k][l].value = 0;
			board[k][l].fixed = 0;
			board[k][l].error = 0;
			board[k][l].options = calloc(size, sizeof(int));
			if(!board[k][l].options)
				{
					printf("Error: calloc has failed\n");
					exit(0);
					return NULL;
				}
			board[k][l].numOfOptions = 0;
		}
	}

	/*now, we are about to create the whole board*/
	newBoard = malloc(sizeof(Board));
	if(!newBoard)
	{
		printf("Error: malloc has failed\n");
		exit(0);
		return NULL;
	}
	/*fill the new board*/
	newBoard->cells = board;
	newBoard->n = n;
	newBoard->m = m;
	newBoard->boardsize = size;

	return newBoard;
}

/*
 * initUserBoard
 *
 *  This function gets an already created board an turns some of the cell in it to fixed,
 *  and deletes everything else.
 *  @param userBoard - the solved board
 *  @param fixedCells - desired amount of fixed cells
 *  @return -
 */
void initUserBoard(Board* userBoard, int fixedCells)
{
	int randRow, randColumn;
	int i, j, size;

	size = userBoard->boardsize;
	while(fixedCells>0)
	{
		randColumn = rand()%size;
		randRow = rand()%size;

		if(userBoard->cells[randRow][randColumn].fixed==0)
		{
			userBoard->cells[randRow][randColumn].fixed=1;
			fixedCells--;
		}

	}

	for(i=0;i<size;i++)
		for(j=0;j<size;j++)
			if(userBoard->cells[i][j].fixed==0)
			{
				userBoard->cells[i][j].value=0;
				/*maybe need to init options*/
			}

}
/*
 * printBoard
 *
 *  This function prints the current status of the sudoku board, according to the desired format
 *  0 counts as empty cell. dot addition represents fixed cell
 *  @param board - the solved board
 *  @return -
 */
void printBoard(Board *board)
{
	/*need to update n,m and N */
	int i,j,k;
	int n,m,N;
	char specialSign;

	/*definitions of dimentions: */
	n=board->n, m=board->m, N=board->boardsize;

	for (i=0; i<N; i++)
	{
		if (i%m==0)
		{
			for (k = 0; k < 4*N + m +1; k++)
			    printf("-");
			printf("\n");
		}

		for (j=0; j<N; j++)
		{
			if (j%n==0)
				printf("|");

			printf(" ");

			/*
			printf(" %d",*((board+i*size)+j));*/

			if (board->cells[i][j].fixed==1)
				specialSign = '.';
			else if (board->cells[i][j].error == 1)
				specialSign = '*';
				else
				    specialSign = ' ';

			if (board->cells[i][j].value==0)
				printf("  %c",specialSign);
			else
				printf("%2d%c",board->cells[i][j].value, specialSign);

		}
		printf("|\n");

	}

    for (k = 0; k < 4*N + m +1; k++)
	    printf("-");
	printf("\n");

}

/*
 * set
 *
 *  This function gets partially solved board, and sets a value in the desired location, if possible
 *  @param board - the user's board
 *  @param x - column number
 *  @param y - row number
 *  @param z - value
 *  @return - 0 if set has failed. 1 if set has succeeded. 2 if puzzle is solved
 */
int set(Board *board, int x, int y, int z)
{
	if(board->cells[x][y].fixed == 1)
	{
		printf("Error: cell is fixed\n");
		return 0;
	}

	if(z == 0)
	{
		board->cells[x][y].value = 0;
		printBoard(board);
		return 1;
	}

	if(isValid(board, x, y, z) == 0)
	{
		printf("Error: value is invalid\n");
		return 0;
	}
	else
	{
		board->cells[x][y].value = z;
		printBoard(board);
		if(isBoardValid(board) == 1)
		{
			printf("Puzzle solved successfully\n");
			return 2;
		}
		return 1;
	}
}
/*
 * hint
 *
 *  This function gets solved board, and gives a hint to the user for a specific location (based on the stored solution
 *  )
 *  @param board - the user's board
 *  @param x - column number
 *  @param y - row number
 *  @return - always 1
 */
int hint(Board *solvedBoard, int x, int y)
{
	printf("Hint: set cell to %d\n", solvedBoard->cells[x][y].value);
	return 1;
}
/*
 * validate
 *
 *  This function validates a specific solution of the user
 *  @param userBoard - the user's board
 *  @param generatedBoard - a possible stored solution
 *  @return - 1 if the solution is valid so far, else 0
 */
int validate(Board *generatedBoard, Board *userBoard)
{
	Board *tempBoard;
	/* for compiler only */
	int tempInt;
	int solveable;

	tempBoard = copyBoard(userBoard);
	tempInt = generatedBoard->cells[0][0].value;
	solveable = detBacktracking(tempBoard);
	if(solveable)
	{
		copyIntoBoard(tempBoard, generatedBoard);
		destroyBoard(tempBoard);
		printf("Validation passed: board is solvable\n");
		tempInt = 1;
		return tempInt;
	}
	else
	{
		destroyBoard(tempBoard);
		printf("Validation failed: board is unsolvable\n");
		return 0;
	}
}

static char digitToChar(int c)
{
	if(c==0)
		return '_';
	else
		return c + '0';
}

void redo(Board* board, List* undoList)
{
	int x,y,z,prevValue,movesNum,i;
	char prevChar,zChar;
	if(undoList->head == NULL)
	{
		printf("Error: no moves to redo\n");
	}
	else
	{
		if(undoList->head->next == NULL && undoList->head->prev == NULL)
		{
			movesNum = undoList->head->movesNum;
			x=undoList->head->moves[0][0];
			y=undoList->head->moves[0][1];
			prevValue = undoList->head->moves[0][2];
			z=undoList->head->moves[0][3];

			if(board->cells[x][y].value != prevValue)
			{
				printf("Error: no moves to redo\n");
			}
			else
			{
				for(i=0;i<movesNum;i++)
				{
					x=undoList->head->moves[i][0];
					y=undoList->head->moves[i][1];
					prevValue = undoList->head->moves[i][2];
					z=undoList->head->moves[i][3];
					board->cells[x][y].value = z;
					prevChar = digitToChar(prevValue);
					zChar = digitToChar(z);
					printf("Redo %d,%d: from %c to %c\n",x,y,prevChar,zChar);
				}
			}
		}
		else
		{
			if(undoList->head->next == NULL)
			{
				printf("Error: no moves to redo\n");
			}
			else if(undoList->head->prev == NULL)
			{
				movesNum = undoList->head->movesNum;

				x=undoList->head->moves[0][0];
				y=undoList->head->moves[0][1];
				prevValue = undoList->head->moves[0][2];
				z=undoList->head->moves[0][3];

				if(board->cells[x][y].value != prevValue)
				{
					undoList->head = undoList->head->next;
				}
				movesNum = undoList->head->movesNum;
				for(i=0;i<movesNum;i++)
				{
					x=undoList->head->moves[i][0];
					y=undoList->head->moves[i][1];
					prevValue = undoList->head->moves[i][2];
					z=undoList->head->moves[i][3];
					board->cells[x][y].value = z;
					prevChar = digitToChar(prevValue);
					zChar = digitToChar(z);
					printf("Redo %d,%d: from %c to %c\n",x,y,prevChar,zChar);
				}

			}
			else
			{
				undoList->head = undoList->head->next;
				movesNum = undoList->head->movesNum;
				for(i=0;i<movesNum;i++)
				{
					x=undoList->head->moves[i][0];
					y=undoList->head->moves[i][1];
					prevValue = undoList->head->moves[i][2];
					z=undoList->head->moves[i][3];
					board->cells[x][y].value = z;
					prevChar = digitToChar(prevValue);
					zChar = digitToChar(z);
					printf("Redo %d,%d: from %c to %c\n",x,y,prevChar,zChar);
				}
			}
		}
	}
}


void undo(Board* board, List* undoList)
{
	int x,y,z,prevValue,movesNum,i;
	char prevChar,zChar;
	if(undoList->head == NULL)
	{
		printf("Error: no moves to undo\n");
	}
	else
	{
		if(undoList->head->prev == NULL)
		{
			/*do it for each move in this node */
			movesNum = undoList->head->movesNum;
			for(i=0;i<movesNum;i++)
			{
				x=undoList->head->moves[i][0];
				y=undoList->head->moves[i][1];
				z=undoList->head->moves[i][2];
				prevValue = undoList->head->moves[i][3];
				if(board->cells[x][y].value == z)
					printf("Error: no moves to undo\n");
				else
				{
					board->cells[x][y].value = z;
					prevChar = digitToChar(prevValue);
					zChar = digitToChar(z);
					printf("Undo %d,%d: from %c to %c\n",x,y,prevChar,zChar);
				}
			}
		}
		else
		{
			movesNum = undoList->head->movesNum;
			for(i=0;i<movesNum;i++)
			{
				x=undoList->head->moves[0][0];
				y=undoList->head->moves[0][1];
				z=undoList->head->moves[0][2];
				prevValue = undoList->head->moves[i][3];
				board->cells[x][y].value = z;
				prevChar = digitToChar(prevValue);
				zChar = digitToChar(z);
				printf("Undo %d,%d: from %c to %c\n",x,y,prevChar,zChar);
			}
			undoList->head = undoList->head->prev;
		}
	}
}

/*
 * restart
 *
 *  This function restarts the game. clear both boards and starts the whole process from the beginning
 *  @param userBoard - the user's board
 *  @param generatedBoard - a possible stored solution
 *  @param size - boards size
 *  @return -
 */
int restart(Board *generatedBoard, Board *userBoard)
{
	int solveable, fixedCells;

	fixedCells = initialization();
	if (fixedCells == -1)
	{
		exitGame(generatedBoard, userBoard);
	}

	resetBoard(generatedBoard);
	solveable = randBacktracking(generatedBoard);
	copyIntoBoard(generatedBoard, userBoard);
	initUserBoard(userBoard, fixedCells);
	printBoard(userBoard);
	return solveable;
}

/*
 * exitGame
 *
 *  This function free all memory block and quits.
 *  @param userBoard - the user's board
 *  @param generatedBoard - a possible stored solution
 *  @param size - boards size
 *  @return -
 */
void exitGame(Board *generatedBoard, Board *userBoard)
{
	printf("Exiting...\n");
	destroyBoard(userBoard);
	destroyBoard(generatedBoard);
	exit(0);
}

/*
 * startGame
 *
 *  This function will initialize the sudoku game for the first time
 *  builds both boards, gets number of fixed cells from user and makes the desired manipulations
 *  on the user's board before the game begins. also, it prints the board's initial status.
 *  @return -
 */
int startGame(int n, int m)
{
	int fixedCells;
	Board *generatedBoard;
	Board *userBoard;
	int solveable;

	fixedCells = initialization();
	if (fixedCells == -1)
	{
		printf("Exiting...\n");
		exit(0);
	}

	generatedBoard = init(n,m);
	solveable = randBacktracking(generatedBoard);
	userBoard = copyBoard(generatedBoard);
	initUserBoard(userBoard, fixedCells);
	printBoard(userBoard);
	read(generatedBoard,userBoard);

	return solveable;
}
