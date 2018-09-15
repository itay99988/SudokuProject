/*
 * Solver Module
 *
 *  This module is in charge of the game's solution. every function which is related to
 *  a specific operation/stage in the solution is here. (such as isValid value for a cell, deterministic BackTracking , randomized backTracking , etc)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "game.h"
#include "stack.h"
#include "mainAux.h"
#include "ILPSolver.h"

/* private methods declaration: */
void markErrors(Board *board, int row, int column);
int findFirstCell(Board* board, int* x, int* y);

/* Public methods: */

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
    	/* check if it is valid value in the row t=and the column */
    	if (board->cells[row][i].value==value || board->cells[i][column].value==value)
            return 0;
    }

    /* check if it is valid value in the box */
    for (i=0;i<m; i++)
        for (j=0;j<n; j++)
        {
        	if (board->cells[modifiedRow+i][modifiedColumn+j].value==value)
                return 0;
        }

    return 1;
}

/*
 * validate
 *
 *  This function gets the actual user board, makes a copy of it and then runs ILP on the copy
 *  in order if the board is solvable.
 *  @param board - the actual game board (not a copy of it)
 *  @return - 1 if solveable, 0 if not.
 */
int validate(Board* board){
	Board* boardCopy = copyBoard(board);
	int result = ilpSolve(boardCopy);
	/* the copyboard function allocates some memory, hence we have to free this memory */
	destroyBoard(boardCopy);
	return result;
}

/*
 * generate
 *
 *  This function gets an empty board in edit mode, randomly chooses X cells and fills them
 *  with valid values, solves the board and then shows only Y random cells out of N^2 available
 *  cells. tries this 1000 times if it fails.
 *
 *  @param board - the actual game board
 *  @param undoList - pointer to the undo list
 *  @param x - as shown in the description
 *  @param y - as shown in the description
 *  @return -1 if function succeeded, 0 if not.
 */
int generate(Board* userBoard, List *undoList, int x, int y){
	/* assuming the board is empty, we are in edit mode, and x,y are valid integers */
	int i,j,l,N,randRow,randCol, chosenValue;
	int pickedXCells=1, isBoardSolvable=1, filledSuccessfully = 0, randIndex=0, changesCount=0;
	int** moves;
	Node* newNode = NULL;

	N=userBoard->boardsize; /* n*m */

	/* in this case the board stays empty anyway */
	if(y==0)
		return 1;
	/* try to fill the board with x randomly chosen cells with randomly chosen legal values
	 * only 1000 times.
	 */
	for(i=0;i<1000;i++){
		for(j=0;j<x;j++){
			/* randomly choose x cells */
			randRow = rand()%N;
			randCol = rand()%N;
			if(userBoard->cells[randRow][randCol].value!=0){
				j--;
				continue;
			}
			/* randomly choose legal values for each cell */
			setOptions(userBoard,randRow,randCol);
			if(userBoard->cells[randRow][randCol].numOfOptions==0){
				pickedXCells = 0;
				break;
			}
			randIndex = rand()%userBoard->cells[randRow][randCol].numOfOptions;
			chosenValue = userBoard->cells[randRow][randCol].options[randIndex];
			userBoard->cells[randRow][randCol].value = chosenValue;
		}

		/* if we didn't succeed in choosing x cells, try again */
		if(!pickedXCells){
			resetBoard(userBoard);
			pickedXCells=1;
			continue;
		}
		/* move on to the next part if and only if the new temp board is solvable */
		isBoardSolvable = ilpSolve(userBoard);
		if(!isBoardSolvable){
			resetBoard(userBoard);
			isBoardSolvable=1;
			continue;
		}
		else{
			filledSuccessfully=1;
			break;
		}
	}
	/* return false if we didn't succeed after 1000 times */
	if(!filledSuccessfully)
		return 0;
	/* randomly choose cells to be removed from the fully solved board */
	for(l=1;l<=(N*N - y);l++){
		randRow = rand()%N;
		randCol = rand()%N;
		if(userBoard->cells[randRow][randCol].value==0){
			l--;
			continue;
		}
		userBoard->cells[randRow][randCol].value = 0;
	}

	/* we have to remember to move that we need since we have to update the undo list */
	moves = malloc(y*sizeof(int*));
	if(!moves){
		printf("Error: malloc has failed\n");
		exit(0);
		return 0;
	}
	for(i=0;i<N;i++){
		for(j=0;j<N;j++){
			if(userBoard->cells[i][j].value!=0){
				insertSingleMove(moves, changesCount, i, j, 0, userBoard->cells[i][j].value);
				changesCount++;
			}
		}
	}
	/*node preparation for the undo list */
	updateMovesInNode(&newNode,moves, y);
	addMove(undoList,newNode);

	return 1;
}

/*
 * autoFill
 *
 *  This function gets the gameboard and the undoList, go over each cell and checks if there's only 1 valid value
 *  for it. if yes - sets it (by push it to the stack and go over the stack after), print the action and insert it
 *  to the undoList (again - by go over the stack)
 *
 *  @param board - the actual game board
 *  @param undoList - pointer to the undo list
 *  @return -
 */
void autoFill(Board *board,List *undoList)
{
	int i,j;
	int N;
	int optionalValue,movesNum,prevValue;
	int theOption = 0;
	int** moves;
	Stack* stack;
	StackNode* poppedNode;
	Node* newNode = NULL;

	/* dimensions definition: */
	N=board->boardsize;

	/*stack*/
	stack = initStack();
	poppedNode = (StackNode*)malloc(sizeof(StackNode));
	if(!poppedNode){
			printf("Error: malloc has failed\n");
			exit(0);
		}

	/* check for each cell if there's only 1 valid value for it */
	for (i=0;i<N; i++){
		for (j=0; j<N; j++){
			if(board->cells[i][j].value!=0)
				continue;
			for (optionalValue = 1; optionalValue <= N; optionalValue++)
				if (isValid(board, i, j, optionalValue)){
					if (theOption == 0){
						theOption = optionalValue;
					}
					else{
						theOption = 0;
						break;
					}
				}

			/*if there's only 1 valid value for the cell, push it to the stack and print the set*/
			if (theOption != 0){
				push(stack,i,j,theOption);
				printf("Cell <%d,%d> set to %d\n",j+1,i+1,theOption);
				theOption = 0;
			}
		}
	}

	movesNum=stack->length;
	moves = malloc(movesNum*sizeof(int*));
	if(!moves){
			printf("Error: malloc has failed\n");
			exit(0);
		}
	/* go over the stack and set the values for the cells */
	while(!isEmpty(stack)){
		pop(stack,poppedNode);
		prevValue = board->cells[poppedNode->column][poppedNode->row].value;
		board->cells[poppedNode->column][poppedNode->row].value = poppedNode->value;
		markErrors(board,poppedNode->column,poppedNode->row);
		insertSingleMove(moves, stack->length, poppedNode->column, poppedNode->row, prevValue, poppedNode->value);
	}
	/* update the list iff the autofill actually did something */
	if(movesNum != 0)
	{
		/*node preparation*/
		updateMovesInNode(&newNode,moves, movesNum);
		addMove(undoList,newNode);
	}
	else{
		free(moves);
	}
	free(poppedNode);
	destroyStack(stack);
}

/*
 * getNumSolutions
 *
 *  This function gets a game board, and returns the number of the valid solutions for this
 *  board. this time we use an iterative solution to solve the problem. we use stack to demonstrate
 *  the recursive calls. we push and pop from the stack depends on the iterations until we count
 *  all the possible solutions
 *
 *  @param board - the actual game board
 *  @return - number of possible solutions
 */
int getNumSolutions(Board* board)
{
	int i=0,j=0,size,foundVal,count;
	StackNode* poppedNode;
	Stack* stack = initStack();

	size=board->boardsize;
	count=0,foundVal=0;
	/* memory allocated for the nodes that are about to be popped */
	poppedNode = malloc(sizeof(StackNode));
	if (poppedNode == NULL) {
		exit(0);
	}
	if(!validate(board))
		return 0;
	/* find the first cell to deal with */
	foundVal = findFirstCell(board, &i, &j);
	/* in case the board is full and valid */
	if(!foundVal)
		return 1;
	push(stack,i,j,1); /* try the first possible value  */
	/* if the stack is not empty, it means that there are still some cases to simulate  */
	while(!isEmpty(stack)){
		foundVal = 0;
		/* if the current assignment is legal, move on to the next empty cell  */
		if(isValid(board,top(stack)->column,top(stack)->row,top(stack)->value)){
			board->cells[top(stack)->column][top(stack)->row].value = top(stack)->value;
			/* find the first cell to deal with */
			foundVal = findFirstCell(board, &i, &j);
			/* if we find another empty cell after the assignment, we go one step further in
			 * our simulated recursion, (push new node the to stack  */
			if(foundVal){
				push(stack,i,j,1);
			}
			else{ /* if we got here, it means that we solved the board*/
				/* increase the counter */
				count = count + 1;
				/* zero the last filled cell */
				board->cells[top(stack)->column][top(stack)->row].value = 0;
				if(top(stack)->value<size){
					stack->currentNode->value = stack->currentNode->value + 1;
				}
				else{
					/* if this is not a valid solution, go back to a previously filled cell */
					while(!isEmpty(stack) && top(stack)->value == size){
						pop(stack,poppedNode);
						if(!isEmpty(stack))
							board->cells[top(stack)->column][top(stack)->row].value = 0;
					}
					/* if stack is not empty, try another possible value for the last filled cell */
					if(!isEmpty(stack))
						top(stack)->value = top(stack)->value + 1;
				}
			}
		}
		else{ /* if the current assignment is illegal, try another assignment of value  */
			if(top(stack)->value<size){
				top(stack)->value = top(stack)->value + 1;
			}
			else{/* if we tried all the options for assignment, go back to to
					a previously filled cell*/
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
	/*free all memory resources that were used in function*/
	free(poppedNode);
	destroyStack(stack);
	return count; /* return the number of possible solutions */
}

/*
 * markErrors
 *
 *  This function is called after a cell was change - therefore gets the gameboard and a specific cell -
 *  meaning row and column. it goes over the row, the column and the block and mark error each cell that was affected by
 *  the set action.
 *
 *  @param board - the actual game board
 *  @param row - the cell's row
 *  @param column - the cell's column
 *  @return -
 */
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

	/*gets the upper left cell of the block - the "first" cell*/
	modifiedRow = (row/m)*m;
	modifiedColumn = (column/n)*n;
    for (i=0;i<boardsize; i++)
    {
    	/*goes over the column - mark and unmark errors if needed*/
    	value = board->cells[row][i].value;
    	if(value !=0)
    	{
    		board->cells[row][i].value = 0; /*change the specific chcecked cell's value to 0 - that's how isValid works*/
        	if (isValid(board, row, i, value)) /*check if the value is valid*/
    			board->cells[row][i].error = 0; /*it is - error = 0*/
        	else
        		board->cells[row][i].error = 1; /*it's not - error = 1*/

        	board->cells[row][i].value = value; /*change it back to the original value*/
    	}
    	else
    		board->cells[row][i].error = 0; /*the value is 0 there for cannot be an error*/

    	/*goes over the row - mark and unmark errors if needed*/
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

    /*goes over the block - mark and unmark errors if needed*/
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


/*
 * isThereAnError
 *
 *  This function gets the gameboard and check if there's any cell marked with error. it assumes the cells are marked
 *  correctly, meaning that markErrors or markAllBoardErrors was called before
 *
 *  @param board - the actual game board
 *  @return - 1 if there's at least 1 error in the board, 0 otherwise
 */
int isThereAnError(Board *board){
	int i,j, size;
	size=board->boardsize;

	for (i=0;i<size; i++)
		for (j=0; j<size; j++)
			if(board->cells[i][j].error == 1)
				return 1;

	return 0;
}

/*
 * markAllBoardErrors
 * REPLACE - check if it is correctly, modified isn't even used
 * REPLACE - document it after
 *
 *  This function gets the gameboard goes over all the cells and update their error value by checking if it is valid
 *
 *  @param board - the actual game board
 *  @return -
 */
void markAllBoardErrors(Board* board){
    int row,column;
    int n,m,boardsize;
    int modifiedRow, modifiedColumn;
    int value;

    modifiedRow = 0; /*just for initialization*/
    modifiedColumn = 0; /*just for initialization*/
    n=modifiedRow*modifiedColumn; /*just for using the variables*/

    /* dimensions definition: */
	n=board->n;
	m=board->m;
	boardsize=board->boardsize;

	/*goes over all the cells in the board*/
	for(row=0;row<boardsize;row++)
		for(column=0;column<boardsize;column++)
		{
			/* only unfixed cells can be erroneous */
			if(!board->cells[row][column].fixed)
			{
				modifiedRow = (row/m)*m;
				modifiedColumn = (column/n)*n;
				value = board->cells[row][column].value;
				board->cells[row][column].value = 0; /*again - sets the value to 0 in order that isValid will work*/
				if (value!=0) /*only cells with values can be erroneous*/
				{
					/*check if it is valid and update the error sign oppositely*/
					board->cells[row][column].error = !isValid(board,row,column,value);
					/*
					if(board->cells[row][column].error==1)
						board->cells[row][column].fixed = 0;
					*/
				}

				board->cells[row][column].value = value; /*change it back to the original value*/
			}

		}
}

/* End of public methods */

/* Private methods: */

/*
 * findFirstCell
 *
 *  This function finds the first empty cell for the num_solution algorithm to deal with
 *  it stores the x,y coordinate of the cell in the x,y parameters, and returns whether or not
 *  this kind of cell actually exists.
 */
int findFirstCell(Board* board, int* x, int* y)
{
	int i=0, j=0, size = board->boardsize, foundVal=0;
	/* find the first cell to deal with */
	for(i=0;i<size;i++){
		for(j=0;j<size;j++)
			if(board->cells[i][j].fixed==0 && board->cells[i][j].value==0)
			{
				foundVal=1;
				*x = i;
				*y = j;
				break;
			}
		if(foundVal)
			break;
	}
	return foundVal;
}

/* End of private methods */
