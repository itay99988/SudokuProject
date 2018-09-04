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
#include "undoList.h"
#include "ILPSolver.h"
#include "tools.h"

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
			else if (board->cells[i][j].error == 1 && board->markErrors == 1)
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
 *  @return - 0 if set has failed. 1 if set has succeeded. 2 if puzzle is solved. 3 if puzzle solution erroneous
 */
int set(Board *board, List *undoList, int x, int y, int z, int gameMode)
{
	int prevValue;
	int* oneMove;
	int** moves;
	Node* newNode;
	if(board->cells[x][y].fixed == 1)
	{
		printf("Error: cell is fixed\n");
		return 0;
	}


	prevValue = board->cells[x][y].value;
	board->cells[x][y].value = z;

	/*node preparation*/
	moves = malloc(sizeof(int*));
	oneMove = malloc(4*sizeof(int));
	newNode = malloc(sizeof(Node));
	if(!moves || !oneMove || !newNode)
	{
		printf("Error: malloc has failed\n");
		exit(0);
		return 0;
	}
	oneMove[0]=x,oneMove[1]=y,oneMove[2]=prevValue,oneMove[3]=z;
	moves[0] = oneMove;
	newNode->moves = moves;
	newNode->movesNum = 1;
	newNode->next = NULL;
	newNode->prev = NULL;

	/* adding the new node to the list */
	addMove(undoList,newNode);

	/* end of node preparation */

	markErrors(board,x,y);
	printBoard(board);


	if (gameMode==1)  /*relevant only to solve mode */
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
 *  This function gets solved board, and gives a hint to the user for a specific location (based on the stored solution
 *  )
 *  @param board - the user's board
 *  @param x - column number
 *  @param y - row number
 *  @return - always 1
 */
void hint(Board *solvedBoard, int x, int y)
{
	printf("Hint: set cell to %d\n", solvedBoard->cells[x][y].value);
}

static char digitToChar(int c)
{
	if(c==0)
		return '_';
	else
		return c + '0';
}

void redo(Board* board, List* undoList, int printVal)
{
	int x,y,z,prevValue,movesNum,i;
	char prevChar,zChar;
	if(undoList->current->next == NULL){
		if(printVal)
			printf("Error: no moves to redo\n");
	}
	else{
		undoList->current = undoList->current->next;
		movesNum = undoList->current->movesNum;
		for(i=0;i<movesNum;i++){
			x=undoList->current->moves[i][0];
			y=undoList->current->moves[i][1];
			prevValue = undoList->current->moves[i][2];
			z=undoList->current->moves[i][3];
			board->cells[x][y].value = z;
			prevChar = digitToChar(prevValue);
			zChar = digitToChar(z);
			markErrors(board, x, y);
			if(printVal)
				printf("Redo %d,%d: from %c to %c\n",y+1,x+1,prevChar,zChar);
		}
	}
}


void undo(Board* board, List* undoList, int printVal)
{
	int x,y,z,prevValue,movesNum,i;
	char prevChar,zChar;
	if(undoList->current->prev == NULL){
		if(printVal)
			printf("Error: no moves to undo\n");
	}
	else{
		movesNum = undoList->current->movesNum;
		for(i=0;i<movesNum;i++){
			x=undoList->current->moves[i][0];
			y=undoList->current->moves[i][1];
			z=undoList->current->moves[i][2];
			prevValue = undoList->current->moves[i][3];
			board->cells[x][y].value = z;
			prevChar = digitToChar(prevValue);
			zChar = digitToChar(z);
			markErrors(board, x, y);
			if(printVal)
				printf("Undo %d,%d: from %c to %c\n",y+1,x+1,prevChar,zChar);
		}
		undoList->current = undoList->current->prev;
	}
}

/* assuming the board is empty, we are in edit mode, and x,y are valid integers */
int generate(Board* userBoard, List *undoList, int x, int y){
	int i,j,l,N,randRow,randCol, chosenValue;
	int pickedXCells=1, isBoardSolvable=1, filledSuccessfully = 0, randIndex=0, changesCount=0;
	int* oneMove;
	int** moves;
	Node* newNode;

	N=userBoard->boardsize;
	if(y==0)
		return 1;

	for(i=0;i<1000;i++){
		for(j=0;j<x;j++){
			randRow = rand()%N;
			randCol = rand()%N;
			if(userBoard->cells[randRow][randCol].value!=0){
				j--;
				continue;
			}
			setOptions(userBoard,randRow,randCol);
			printArray(userBoard->cells[randRow][randCol].options,userBoard->cells[randRow][randCol].numOfOptions);
			if(userBoard->cells[randRow][randCol].numOfOptions==0){
				pickedXCells = 0;
				break;
			}
			randIndex = rand()%userBoard->cells[randRow][randCol].numOfOptions;
			chosenValue = userBoard->cells[randRow][randCol].options[randIndex];
			userBoard->cells[randRow][randCol].value = chosenValue;
		}

		if(!pickedXCells){
			resetBoard(userBoard);
			pickedXCells=1;
			continue;
		}

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
	if(!filledSuccessfully)
		return 0;

	for(l=1;l<=(N*N - y);l++){
		randRow = rand()%N;
		randCol = rand()%N;
		if(userBoard->cells[randRow][randCol].value==0){
			l--;
			continue;
		}
		userBoard->cells[randRow][randCol].value = 0;
	}

	moves = malloc(y*sizeof(int*));
	if(!moves){
		printf("Error: malloc has failed\n");
		exit(0);
		return 0;
	}
	for(i=0;i<N;i++){
		for(j=0;j<N;j++){
			if(userBoard->cells[i][j].value!=0){
				oneMove = malloc(4*sizeof(int));
				if(!oneMove){
					printf("Error: malloc has failed\n");
					exit(0);
					return 0;
				}
				oneMove[0]=i; oneMove[1]=j; oneMove[2]=0; oneMove[3]=userBoard->cells[i][j].value;
				moves[changesCount] = oneMove;
				changesCount++;
			}
		}
	}

	/*node preparation*/
	newNode = malloc(sizeof(Node));
	if(!newNode){
		printf("Error: malloc has failed\n");
		exit(0);
		return 0;
	}
	newNode->moves = moves;
	newNode->movesNum = y;
	newNode->next = NULL;
	newNode->prev = NULL;
	/* adding the new node to the list */
	addMove(undoList,newNode);
	/* end of node preparation */

	return 1;
}

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
 *  @param generatedBoard - a possible stored solution
 *  @param size - boards size
 *  @return -
 */
void exitGame(Board *userBoard, List *undoList)
{
	printf("Exiting...\n");

	destroyList(undoList);
	destroyBoard(userBoard);

	exit(0);
}

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

/*
 * validate
 *
 *  This function gets the actual user board, makes a copy of it and then runs ILP on the copy
 *  in order if the board is solvable.
 *  @param board - the actual game board (not a copy of it)
 *  @return -1 if solveable, 0 if not
 */
int validate(Board* board){
	Board* boardCopy = copyBoard(board);
	int result = ilpSolve(boardCopy);
	/* the copyboard function allocates some memory, hence we have to free this memory */
	destroyBoard(boardCopy);
	return result;
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

void doGenerate(Board* userBoard, List* undoList, int x, int y){
	int result;

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

/*
 * startGame
 *
 *  This function will initialize the sudoku game for the first time
 *  builds both boards, gets number of fixed cells from user and makes the desired manipulations
 *  on the user's board before the game begins. also, it prints the board's initial status.
 *  @return -
 */
void startGame()
{
	printf("Sudoku\n------\n");
	read();
}
