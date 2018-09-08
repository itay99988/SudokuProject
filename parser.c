/*
 * Parser Module
 *
 *  This module is in charge of writing and reading the commands. it has two functions - initialization and read
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "game.h"
#include "solver.h"
#include "undoList.h"
#include "tools.h"
#include "mainAux.h"
#include "ILPSolver.h"

/* Private methods declaration */
int isInt(char* string);

/* Public methods: */

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
	List* undoList;
	int i = 0;
	int solved = 0; /*'set' returns 2 if puzzle was solved*/
	Board* userBoard;

	/*const int initialBoardDimension = 3;  according to the forum  is 3*3 - want to change it to define???, was moved to game.c*/


	/* dimensions definition: */
	userBoard = NULL;
	undoList = NULL;

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
			  		solved = doSet(userBoard,undoList, string[1], string[2], string[3],mode);

					if (solved == 2)
					{
						mode = 0;
						solved = 0;
					}
				}
				else if (strcmp(string[0],"hint")==0 && string[1]!=NULL && isInt(string[1]) && string[2]!=NULL && isInt(string[2]) && solved!=2 && mode==1) /*available only in solve*/
					{/*need to update according to new rules*/
					doHint(userBoard, string[1],string[2]);
					}

				else if (strcmp(string[0],"validate")==0 && solved!=2 && (mode==1 || mode==2))/*available in solve or edit*/
					{
						doValidate(userBoard);
					}

				else if (strcmp(string[0],"reset")==0 && (mode==1 || mode==2))/*available in solve or edit*/
				{
					reset(userBoard,&undoList);
					solved = 0;
				}

				else if (strcmp(string[0],"solve")==0 && string[1]!=NULL) /*available in every mode*/
				{
					mode = 1;
					doSolve(string[1], &userBoard, &undoList, mode, (userBoard ? userBoard->markErrors : 1));
				}
				else if (strcmp(string[0],"edit")==0) /*available in every mode*/
				{
					/* implement edit*/
					mode = 2; /* start a puzzle in edit mode */
					doEdit(string[1],&userBoard, &undoList, mode);
				}
				else if (strcmp(string[0],"mark_errors")==0 && string[1]!=NULL && mode==1) /*available only in solve*/
				{
					doMarkErrors(userBoard, string[1]);
				}
				else if (strcmp(string[0],"print_board")==0 && (mode==1 || mode==2)) /*available in solve or edit*/
				{
					printBoard(userBoard);
				}
				else if (strcmp(string[0],"generate")==0 && string[1]!=NULL && isInt(string[1]) && string[2]!=NULL && isInt(string[2]) && mode==2) /*available only in edit*/
				{
					doGenerate(userBoard, undoList, string[1], string[2]);
				}
				else if (strcmp(string[0],"undo")==0 && (mode==1 || mode==2)) /*available in solve or edit*/
				{
					undo(userBoard,undoList,1);
				}
				else if (strcmp(string[0],"redo")==0 && (mode==1 || mode==2)) /*available in solve or edit*/
				{
					redo(userBoard,undoList,1);
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
				{
					printf("Error: invalid command\n");
				}

			  	printf("Enter your command:\n");
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

/* End of public methods */


/* Private methods: */

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

