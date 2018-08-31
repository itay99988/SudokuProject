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
#include "stack.h"

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

void autoFill(Board *board,List *undoList)
{
	int i,j;
	int N;
	int optionalValue,movesNum,prevValue;
	int theOption = 0;
	int* oneMove;
	int** moves;
	Stack* stack;
	StackNode* poppedNode;
	Node* newNode;

	/* dimensions definition: */
	N=board->boardsize;

	/*stack*/
	stack = initStack();
	poppedNode = (StackNode*)malloc(sizeof(StackNode));

	for (i=0;i<N; i++)
	{
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
			{
				push(stack,i,j,theOption);
				printf("Cell <%d,%d> set to %d\n",i,j,theOption);
			}
		}
	}

	movesNum=stack->length;
	moves = malloc(movesNum*sizeof(int*));
	while(!isEmpty(stack))
	{
		pop(stack,poppedNode);
		prevValue = board->cells[poppedNode->column][poppedNode->row].value;
		board->cells[poppedNode->column][poppedNode->row].value = poppedNode->value;

		oneMove = malloc(4*sizeof(int));
		oneMove[0]=poppedNode->column,oneMove[1]=poppedNode->row,oneMove[2]=prevValue,oneMove[3]=poppedNode->value;
		moves[stack->length] = oneMove;
	}

	/*node preparation*/
	newNode = malloc(sizeof(Node));
	newNode->moves = moves;
	newNode->movesNum = movesNum;
	newNode->next = NULL;
	newNode->prev = NULL;

	/* adding the new node to the list */
	addMove(undoList,newNode);

	/* end of node preparation */

	free(poppedNode);
	destroyStack(stack);
}

int countDetBacktracking(Board* board)
{
	int i,j,k;
	int size;
	int count;
	count=0;

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
						count = count + countDetBacktracking(board);
					}
				}

				board->cells[i][j].value=0;
				return count;
			}
		}
	}

	return 1;
}

int getNumSolutions(Board* board)
{
	int i,j,size,foundVal,count;
	StackNode* poppedNode;
	Stack* stack = initStack();

	size=board->boardsize;
	count=0,foundVal=0;
	poppedNode = malloc(sizeof(StackNode));
	if (poppedNode == NULL) {
		exit(0);
	}
	/* find the first cell to deal with */
	for(i=0;i<size;i++){
		for(j=0;j<size;j++)
			if(board->cells[i][j].fixed==0 && board->cells[i][j].value==0)
			{
				foundVal=1;
				break;
			}
		if(foundVal)
			break;
	}
	push(stack,i,j,1);

	while(!isEmpty(stack)){
		foundVal = 0;
		if(isValid(board,top(stack)->column,top(stack)->row,top(stack)->value)){
			board->cells[top(stack)->column][top(stack)->row].value = top(stack)->value;
			/* find the first cell to deal with */
			for(i=0;i<size;i++){
				for(j=0;j<size;j++){
					if(board->cells[i][j].fixed==0 && board->cells[i][j].value==0)
					{
						foundVal=1;
						break;
					}
				}
				if(foundVal)
					break;
			}
			if(foundVal){
				push(stack,i,j,1);
			}
			else{
				count = count + 1;
				board->cells[top(stack)->column][top(stack)->row].value = 0;
				if(top(stack)->value<size){
					stack->currentNode->value = stack->currentNode->value + 1;
				}
				else{
					while(!isEmpty(stack) && top(stack)->value == size){
						pop(stack,poppedNode);
						if(!isEmpty(stack))
							board->cells[top(stack)->column][top(stack)->row].value = 0;
					}
					if(!isEmpty(stack))
						top(stack)->value = top(stack)->value + 1;
				}
			}
		}
		else{
			if(top(stack)->value<size){
				top(stack)->value = top(stack)->value + 1;
			}
			else{
				while(!isEmpty(stack) && top(stack)->value == size){
					pop(stack,poppedNode);
					if(stack->length>0)
						board->cells[top(stack)->column][top(stack)->row].value = 0;

				}
				if(!isEmpty(stack))
					top(stack)->value = top(stack)->value + 1;
			}
		}
	}

	printf("Number of solutions: %d\n",count);
	if(count == 1)
		printf("This is a good board!\n");
	else
		printf("The puzzle has more than 1 solution, try to edit it further\n");
	free(poppedNode);
	destroyStack(stack);
	return count;
}

void markErrors(Board *board, int row, int column)
{
    int i,j;
    int n,m,boardsize;
    int modifiedRow, modifiedColumn;
    int value;

    /* dimensions definition: */
	n=board->n;
	m=board->m;
	boardsize=board->boardsize;

	modifiedRow = (row/m)*m;
	modifiedColumn = (column/n)*n;
    for (i=0;i<boardsize; i++)
    {
    	value = board->cells[row][i].value;
    	if(value !=0)
    	{
    		board->cells[row][i].value = 0;
        	if (isValid(board, row, i, value))
    			board->cells[row][i].error = 0;
        	else
        		board->cells[row][i].error = 1;

        	board->cells[row][i].value = value;
    	}
    	else
    		board->cells[row][i].error = 0;

    	value = board->cells[i][column].value;
    	if(value !=0)
    	{
    		board->cells[i][column].value = 0;
        	if (isValid(board, i, column, value))
    			board->cells[i][column].error = 0;
        	else
        		board->cells[i][column].error = 1;

        	board->cells[i][column].value = value;
    	}
    	else
    		board->cells[i][column].error = 0;
    }

    for (i=0;i<m; i++)
        for (j=0;j<n; j++)
        {

        	value = board->cells[modifiedRow+i][modifiedColumn+j].value;
        	if(value !=0)
        	{
        		board->cells[modifiedRow+i][modifiedColumn+j].value = 0;
        		if (isValid(board,modifiedRow+i,modifiedColumn+j, value))
        			board->cells[modifiedRow+i][modifiedColumn+j].error = 0;
            	else
            		board->cells[modifiedRow+i][modifiedColumn+j].error = 1;

        		board->cells[modifiedRow+i][modifiedColumn+j].value = value;
        	}
        	else
        		board->cells[modifiedRow+i][modifiedColumn+j].error = 0;
        }
}


int isThereAnError(Board *board){
	int i,j, size;
	size=board->boardsize;

	for (i=0;i<size; i++)
		for (j=0; j<size; j++)
			if(board->cells[i][j].error == 1)
				return 1;

	return 0;
}
