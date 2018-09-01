/*
 * Parser Module
 *
 *  This module is in charge of writing and reading the commands. it has two functions - initialization and read
 */

#include <stdio.h>
#include <stdlib.h>
#include "SPBufferset.h"
#include <string.h>
#include <ctype.h>
#include "game.h"
#include "solver.h"
#include "undoList.h"
#include "tools.h"
#include "solver.h"
#include "mainAux.h"
#include "ILPSolver.h"





int isInt(char *string)
{
        int i, stringLength = strlen(string);

        for(i = 0; i < stringLength; i++)
        {
                if(isdigit(string[i]) == 0) /*|| ispunct(string[i]) != 0 ... */
                        break;
        }

        if(i != stringLength)
                return 0;
        else
                return 1;
}


/*
 * read
 *
 *  This function reads the user's game-plays and interpret them - calls the right function in game
 *  @param generatedBoard - a possible stored solution
 *  @param userBoard - the user's board
 *  @return -
 */
void read()
{
	/* Game mode:
	 * 0 - Init
	 * 1 - Solve
	 * 2 - Edit
	 */
	int mode = 0; /*starts in Init mode*/
	char input[256];
	char *string[256];

	char delimiters[] = " \t\r\n";
	int x,y,z;
	int boardsize;
	List* undoList = initList();
	int i = 0;
	int solved = 0; /*'set' returns 2 if puzzle was solved*/
	Board* userBoard;
	Board* fullBoard;

	/*const int initialBoardDimension = 3;  according to the forum  is 3*3 - want to change it to define???, was moved to game.c*/


	/* dimensions definition: */
	boardsize = 9; /*will be changed*/
	userBoard = init(3,3); /*'userBoard' may be used uninitialized...*/

	printf("Enter your command:\n");
	if(fgets(input, sizeof input, stdin)!=NULL)
	{
		string[i]=strtok(input,delimiters);

		while(string[i]!=NULL)
		{
		  /*printf("string [%d]=%s\n",i,string[i]);*/
		  i++;
		  string[i]=strtok(NULL,delimiters);
		}
	}
	else
	{
		  /*GOT EOF, therefore exits*/
		  /*exitGame(userBoard);*/
		exit(0);
	}

	  while(string[0]== '\0' || strcmp(string[0],"exit")!=0)
	  {
		  if(string[0]!='\0')
		  {
			  	if ((strcmp(string[0],"set")==0) && string[1]!=NULL && isInt(string[1]) && string[2]!=NULL && isInt(string[2]) && string[3]!=NULL && isInt(string[3]) && (solved!=2) && (mode==1 || mode==2)) /*available in solve or edit*/
				{	/*need to update according to new rules - check if solved is still relevant*/
					x = atoi(string[1]);
					y = atoi(string[2]);
					z = atoi(string[3]);

					boardsize = userBoard->boardsize; /*do we need it??/*/

					if((x==0 && strcmp(string[1],"0")!=0)||(y==0 && strcmp(string[2],"0")!=0)||(z==0 && strcmp(string[3],"0")!=0))
							printf("Error: value not in range 0-%d\n",boardsize);
					else if (!((x>=1 && x<=boardsize) && (y>=1 && y<=boardsize) && (z>=0 && z<=boardsize)))
						printf("Error: value not in range 0-%d\n",boardsize);
					else
					{
						solved = set(userBoard, undoList ,y-1,x-1,z, mode);

						if (solved == 2)
						{
							mode = 0; /* puzzle solved, game mode is INIT */
							solved = 0;
						}
					}
				}
				else if (strcmp(string[0],"hint")==0 && string[1]!=NULL && isInt(string[1]) && string[2]!=NULL && isInt(string[2]) && solved!=2 && mode==1) /*available only in solve*/
					{/*need to update according to new rules*/
						x = atoi(string[1]);
						y = atoi(string[2]);
						boardsize = userBoard->boardsize; /*do we need it??/*/

						if((x==0 && strcmp(string[1],"0")!=0)||(y==0 && strcmp(string[2],"0")!=0))
								printf("Error: value not in range 1-%d\n",boardsize);
						else if (!((x>=1 && x<=boardsize) && (y>=1 && y<=boardsize)))
							printf("Error: value not in range 1-%d\n",boardsize);
						else if (isThereAnError(userBoard)){
							printf("Error: board contains erroneous values\n");
						}
						else
						{
							if (userBoard->cells[y-1][x-1].fixed==1)
								printf("Error: cell is fixed\n");
							else if (userBoard->cells[y-1][x-1].value!=0)
								printf("Error: cell already contains a value\n");
							else
							{
								/*run ILP and get...*/
								fullBoard = copyBoard(userBoard);
								solved = ilpSolve(fullBoard);
								if (solved==0)
									printf("Error: board is unsolvable\n");
								else
									hint(fullBoard,y-1,x-1);
								/*free the solved board's memory*/
								destroyBoard(fullBoard);
							}
						}
					}

				else if (strcmp(string[0],"validate")==0 && solved!=2 && (mode==1 || mode==2))/*available in solve or edit*/
					{
						doValidate(userBoard);
					}

				else if (strcmp(string[0],"reset")==0 && (mode==1 || mode==2))/*available in solve or edit*/
				{
					reset(userBoard,undoList);
					solved = 0;
				}

				else if (strcmp(string[0],"solve")==0 && string[1]!=NULL) /*available in every mode*/
				{
					mode = 1;
					userBoard = doSolve(string[1], undoList,mode, userBoard->markErrors);
				}
				else if (strcmp(string[0],"edit")==0) /*available in every mode*/
				{
					/* implement edit*/
					mode = 2; /* start a puzzle in edit mode */
					userBoard = doEdit(string[1], undoList, mode);
				}
				else if (strcmp(string[0],"mark_errors")==0 && string[1]!=NULL && mode==1) /*available only in solve*/
				{
					/* implement mark errors*/
					if (strcmp(string[1],"1")==0 || strcmp(string[1],"0")==0)
					{
						userBoard ->markErrors = atoi(string[1]);
					}
					else
						printf("Error: the value should be 0 or 1\n");
				}
				else if (strcmp(string[0],"print_board")==0 && (mode==1 || mode==2)) /*available in solve or edit*/
				{
					/* implement printBoard*/
					printBoard(userBoard);
				}
				else if (strcmp(string[0],"generate")==0 && string[1]!=NULL && isInt(string[1]) && string[2]!=NULL && isInt(string[2]) && mode==2) /*available only in edit*/
				{
					/* implement generate*/
					x = atoi(string[1]);
					y = atoi(string[2]);
					/* need to update empty cells according to d */
					boardsize = userBoard->boardsize; /*do we need it??/*/

					/* need to check - d!!! */
					if((x==0 && strcmp(string[1],"0")!=0)||(y==0 && strcmp(string[2],"0")!=0))
							printf("Error: value not in range 0-%d\n",boardsize*boardsize); /*maybe need to change boardsize*/
					else if ((x>=0 && x<=boardsize*boardsize) && (y>=0 && y<=boardsize*boardsize) && (z>=0 && z<=boardsize*boardsize)) /*maybe need to change boardsize*/
						printf("Error: value not in range 0-%d\n",boardsize*boardsize); /*fixxxxx this!!!! */
					/* ------------------ */
					else
					{
						/*check if the board is empty*/
						if(1)
						{
							/* implement e-h */
						}
						else
							printf("Error: board is not empty\n");
					}
				}
				else if (strcmp(string[0],"undo")==0 && (mode==1 || mode==2)) /*available in solve or edit*/
				{
					undo(userBoard,undoList);
				}
				else if (strcmp(string[0],"redo")==0 && (mode==1 || mode==2)) /*available in solve or edit*/
				{
					redo(userBoard,undoList);
				}
				else if (strcmp(string[0],"save")==0 && string[1]!=NULL && (mode==1 || mode==2)) /*available in solve or edit*/
				{
					doSave(userBoard, string[1], mode);
				}
				else if (strcmp(string[0],"num_solutions")==0 && (mode==1 || mode==2))/*available in solve or edit*/
				{
					doNumSolutions(userBoard);
				}
				else if ((strcmp(string[0],"autofill")==0) && (mode==1)) /*available only in solve*/
				{
					doAutoFill(userBoard, undoList);
				}
				else if (strcmp(string[0],"exit")==0) /*available in every mode*/
				{
					/* implement exit*/
					exitGame(userBoard, undoList);
				}
				else
				{
					printf("Error: invalid command\n");
					/*invalid = 1;*/
				}

			  	printf("Enter your command:\n");
			  	/*
			  	if (!invalid)
			  		printBoard(userBoard);

			  	invalid = 0;*/ /*reset the invalid*/
		  }

		  i=0;
		  if(fgets(input, sizeof input, stdin)!=NULL)
		  {
				string[i]=strtok(input,delimiters);
				while(string[i]!=NULL)
				{
				  /*printf("string [%d]=%s\n",i,string[i]);*/
				  i++;
				  string[i]=strtok(NULL,delimiters);
				}
		  }
		  else
		  {
			  /*GOT EOF, therefore exits*/
			  exitGame(userBoard, undoList);
		  }

	  }

	  /*NEED TO EXIT*/
	  exitGame(userBoard, undoList);

}

