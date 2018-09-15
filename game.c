/*
 * Game Module
 *
 *  This module is in charge of game-play. every function which is related to
 *  a specific operation/stage in the game is here. (such as initialize board, set,hint,validate etc)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "undoList.h"
#include "game.h"
#include "mainAux.h"
#include "solver.h"
#include "parser.h"

/* Public methods: */

/*
 * init
 *
 *  This function initializes a board with default values
 *  @param n,m - n,m fields of the board
 *  @return - Board* - the initiate Board
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
		{  /*the initial values of the cell*/
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
	newBoard->markErrors = 1;
	newBoard->cells = board;
	newBoard->n = n;
	newBoard->m = m;
	newBoard->boardsize = size;

	return newBoard;
}

/*
 * printBoard
 *
 *  This function prints the current status of the sudoku board, according to the desired format
 *  0 counts as empty cell, dot addition represents fixed cell, asterisk addition represents erroneous cell
 *  @param board - the solved board
 *  @return -
 */
void printBoard(Board *board)
{
	/*need to update n,m and N */
	int i,j,k;
	int n,m,N;
	char specialSign;

	/*definitions of dimensions: */
	n=board->n, m=board->m, N=board->boardsize;
	for (i=0; i<N; i++)
	{
		if (i%m==0)
		{	/*prints the ----- */
			for (k = 0; k < 4*N + m +1; k++)
			    printf("-");
			printf("\n");
		}

		for (j=0; j<N; j++)
		{
			if (j%n==0)
				printf("|");

			printf(" ");

			if (board->cells[i][j].fixed==1)
				specialSign = '.';
			else if (board->cells[i][j].error == 1 && board->markErrors == 1)
				specialSign = '*';
				else
				    specialSign = ' ';

			/*prints specia sign if needed - error or fixed*/
			if (board->cells[i][j].value==0)
				printf("  %c",specialSign);
			else
				printf("%2d%c",board->cells[i][j].value, specialSign);

		}
		printf("|\n");

	}
	/*prints the ----- */
    for (k = 0; k < 4*N + m +1; k++)
	    printf("-");
	printf("\n");
}

/*
 * set
 *
 *  This function gets partially solved board, and sets a value in the desired location, if possible
 *  @param board - the user's board
 *  @param list - the doubly linked list which stores the moves
 *  @param x - column number
 *  @param y - row number
 *  @param z - value
 *  @param gameMode - the current mode of the game
 *  @return - 0 if set has failed. 1 if set has succeeded. 2 if puzzle is solved successfully.
 */
int set(Board *board, List *undoList, int x, int y, int z, int gameMode)
{
	int prevValue;
	int** moves;
	Node* newNode = NULL;
	/* cannot set if fixed */
	if(board->cells[x][y].fixed == 1)
	{
		printf("Error: cell is fixed\n");
		return 0;
	}

	prevValue = board->cells[x][y].value;
	board->cells[x][y].value = z;

	/*node preparation*/
	moves = malloc(sizeof(int*));
	if(!moves)
	{
		printf("Error: malloc has failed\n");
		exit(0);
		return 0;
	}
	insertSingleMove(moves, 0, x, y, prevValue, z);
	updateMovesInNode(&newNode,moves, 1);
	addMove(undoList,newNode);
	/* end of node preparation */

	markErrors(board,x,y);
	printBoard(board);

	if (gameMode==1)  /*checking if game is finished - relevant only to solve mode */
	{
		if(isBoardFull(board))
		{
			if (isThereAnError(board))
				printf("Puzzle solution erroneous\n");
			else
			{
				printf("Puzzle solved successfully\n");
				return 2;
			}
		}
	}
	return 1;
}
/*
 * hint
 *
 *  This function gets solved board, and prints a hint to the user for a specific location (based on the stored solution)
 *  @param solvedBoard - a solve board, according to the user's board
 *  @param x - column number
 *  @param y - row number
 *  @return -
 */
void hint(Board *solvedBoard, int x, int y)
{
	printf("Hint: set cell to %d\n", solvedBoard->cells[x][y].value);
}

/*
 * redo
 *
 *  This function redoes the last undone operation of user that affected the board.
 *
 *  @param board - pointer to the game board
 *  @param undoList - the doubly linked list which stores the moves
 *  @param printVal - whether of not the function print its actions
 *  @param mode - a pointer to the current game mode
 *  @return -
 */
void redo(Board* board, List* undoList, int printVal, int* mode)
{
	int x,y,z,prevValue,movesNum,i;
	/* if we are pointing on the last move done by the user, then no moves to undo */
	if(undoList->current->next == NULL){
		if(printVal)
			printf("Error: no moves to redo\n");
	}
	else{
		/* go to the next node in the list */
		undoList->current = undoList->current->next;
		/* check how many moves were in the last user's turn */
		movesNum = undoList->current->movesNum;
		for(i=0;i<movesNum;i++){
			/* for each move in the 2d moves array, store move data temporarely */
			x=undoList->current->moves[i][0];
			y=undoList->current->moves[i][1];
			prevValue = undoList->current->moves[i][2];
			z=undoList->current->moves[i][3];
			/* update the game board accordingly (for each single move in user's turn)*/
			board->cells[x][y].value = z;
			/* check for errors */
			markErrors(board, x, y);
			if(printVal) /* print only if we need */
			{
				if(prevValue==0 && z==0)
					printf("Redo %d,%d: from _ to _\n",y+1,x+1);
				else if (prevValue==0)
					printf("Redo %d,%d: from _ to %d\n",y+1,x+1, z);
				else if (z==0)
					printf("Redo %d,%d: from %d to _\n",y+1,x+1, prevValue);
				else
					printf("Redo %d,%d: from %d to %d\n",y+1,x+1, prevValue, z);
			}


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
	}
}

/*
 * undo
 *
 *  This function undoes the last operation of user that affected the board.
 *
 *  @param board - pointer to the game board
 *  @param undoList - the doubly linked list which stores the moves
 *  @param printVal - whether of not the function print its actions
 *  @return -
 */
void undo(Board* board, List* undoList, int printVal)
{
	int x,y,z,prevValue,movesNum,i;
	/* if we are pointing on the first move done by the user, then no moves to undo */
	if(undoList->current->prev == NULL){
		if(printVal)
			printf("Error: no moves to undo\n");
	}
	else{
		/* check how many moves were in the last user's turn */
		movesNum = undoList->current->movesNum;
		for(i=0;i<movesNum;i++){
			/* for each move in the 2d moves array, store move data temporarely */
			x=undoList->current->moves[i][0];
			y=undoList->current->moves[i][1];
			z=undoList->current->moves[i][2];
			prevValue = undoList->current->moves[i][3];
			/* update the game board accordingly (for each single move in user's turn)*/
			board->cells[x][y].value = z;
			/* check for errors */
			markErrors(board, x, y);
			if(printVal) /* print only if we need */
			{
				if(prevValue==0 && z==0)
					printf("Undo %d,%d: from _ to _\n",y+1,x+1);
				else if (prevValue==0)
					printf("Undo %d,%d: from _ to %d\n",y+1,x+1, z);
				else if (z==0)
					printf("Undo %d,%d: from %d to _\n",y+1,x+1, prevValue);
				else
					printf("Undo %d,%d: from %d to %d\n",y+1,x+1, prevValue, z);
			}
		}
		undoList->current = undoList->current->prev; /* go to the previous node */
	}
}

/*
 * reset
 *
 *  This function reset the game board to his first stage, meaning undo all the moves that were done by the user
 *  @param userBoard - the user's board
 *  @param undoList - a pointer to the doubly linked list which stores the moves
 *  @return -
 */
void reset(Board* board, List** undoList){
	while((*undoList)->current->prev != NULL){
		undo(board,*undoList,0);
	}
	destroyList(*undoList);
	*undoList = initList();
	printf("Board reset\n");
}


/*
 * exitGame
 *
 *  This function free all memory block and quits.
 *  @param userBoard - the user's board
 *  @param undoList - the doubly linked list which stores the moves
 *  @return -
 */
void exitGame(Board *userBoard, List *undoList)
{
	printf("Exiting...\n");

	destroyList(undoList);
	destroyBoard(userBoard);

	exit(0);
}

/*
 * startGame
 *
 *  This function will initialize the sudoku game for the first time
 *  prints the desired "line" and call the read function
 *  @return -
 */
void startGame()
{
	printf("Sudoku\n------\n");
	read();
}

/* End of public methods */
