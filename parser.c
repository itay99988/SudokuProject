/*
 * Parser Module
 *
 *  This module is in charge of writing and reading the commands. it has two functions - initialization and read
 */

#include <stdio.h>
#include <stdlib.h>
#include "SPBufferset.h"
#include <string.h>
#include "game.h"
#include "solver.h"
#include "undoList.h"
#include "tools.h"

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
	int mode = 1; /*starts in Init mode*/
	char input[1024]; /*maybe 256?*/
	char *string[1024];

	char delimiters[] = " \t\r\n";
	int x,y,z;
	int boardsize;
	List* undoList = initList();
	int i = 0;
	int solved = 0; /*'set' returns 2 if puzzle was solved*/
	FILE *fp;
	Board* userBoard;


	/* dimensions definition: */
	boardsize = 9; /*will be changed*/

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
			  	if ((strcmp(string[0],"set")==0) && (string[1]!=NULL) && (string[2]!=NULL) && (string[3]!=NULL) && (solved!=2) && (mode==1 || mode==2)) /*available in solve or edit*/
				{	/*need to update according to new rules - check if solved is still relevant*/
					x = atoi(string[1]);
					y = atoi(string[2]);
					z = atoi(string[3]);

					if((x==0 && strcmp(string[1],"0")!=0)||(y==0 && strcmp(string[2],"0")!=0)||(z==0 && strcmp(string[3],"0")!=0))
							printf("Error: value not in range 0-%d\n",boardsize);
					else if (!((x>=0 && x<=boardsize) && (y>=0 && y<=boardsize) && (z>=0 && z<=boardsize)))
						printf("Error: value not in range 0-%d\n",boardsize);
					else
					{
						solved = set(userBoard, undoList ,y-1,x-1,z);
						/*implement f-i*/
					}
				}
				else if (strcmp(string[0],"hint")==0 && string[1]!=NULL && string[2]!=NULL && solved!=2 && mode==1) /*available only in solve*/
					{/*need to update according to new rules*/
						x = atoi(string[1]);
						y = atoi(string[2]);

						if((x==0 && strcmp(string[1],"0")!=0)||(y==0 && strcmp(string[2],"0")!=0))
								printf("Error: value not in range 0-%d\n",boardsize);
						else if (!((x>=0 && x<=boardsize) && (y>=0 && y<=boardsize)))
							printf("Error: value not in range 0-%d\n",boardsize);
						else
						{
							/*hint(generatedBoard, y-1,x-1);*/
							/*implement e-h*/
						}
					}

				else if (strcmp(string[0],"validate")==0 && solved!=2 && (mode==1 || mode==2))/*available in solve or edit*/
					{	/*need to update according to new rules*/
						if(isThereAnError(userBoard))
							printf("Error: board contains erroneous values\n");

						/*need to update the method isBoardValid(userBoard)) according to new rules*/
						if(isBoardValid(userBoard))
							printf("Validation passed: board is solvable\n");
						else
							printf("Validation failed: board is unsolvable\n");						/*printf("VALIDATE");*/

							/*implement validate*/
					}

				else if (strcmp(string[0],"reset")==0 && (mode==1 || mode==2))/*available in solve or edit*/
				{
					/*need to update according to new rules*/

					/*need to update solved to 0*/
					solved = 0;
				}

				else if (strcmp(string[0],"solve")==0 && string[1]!=NULL) /*available in every mode*/
				{
					/* implement solve*/
					mode = 1;
					fp = fopen(string[1], "r");
					if (fp==NULL)
					{
						printf("Error: File doesn't exist or cannot be opened\n");
					}
					else
					{
						/*load*/
						userBoard = load(string[1]);
						printBoard(userBoard);
					}

				}
				else if (strcmp(string[0],"edit")==0) /*available in every mode*/
				{
					/* implement edit*/
					mode = 2;
					userBoard ->markErrors = 1;
					if (string[1]!=NULL) /*there is a parameter*/
					{
						fp = fopen(string[1], "r");
						if (fp==NULL)
						{
							printf("Error: File doesn't exist or cannot be opened\n");
						}
						else
							/*need to call to Solve\load*/
							userBoard = load(string[1]);
					}
					else
					{
						/* need to initilalize an empty board */

					}
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
				}
				else if (strcmp(string[0],"generate")==0 && string[1]!=NULL && string[2]!=NULL && mode==2) /*available only in edit*/
				{
					/* implement generate*/
					x = atoi(string[1]);
					y = atoi(string[2]);
					/* need to update empty cells according to d */

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
					/* implement undo*/
				}
				else if (strcmp(string[0],"redo")==0 && (mode==1 || mode==2)) /*available in solve or edit*/
				{
					/* implement redo*/
				}
				else if (strcmp(string[0],"save")==0 && string[1]!=NULL && (mode==1 || mode==2)) /*available in solve or edit*/
				{
					/* implement save*/
				}
				else if (strcmp(string[0],"num_solutions")==0 && (mode==1 || mode==2))/*available in solve or edit*/
				{
					/* implement c-f*/
				}
				else if ((strcmp(string[0],"autofill")==0) && (mode==1)) /*available only in solve*/
				{
					/* implement c-g*/
				}
				else if (strcmp(string[0],"exit")==0) /*available in every mode*/
				{
					/* implement exit*/
					exitGame(userBoard);
				}
				else
				{
					printf("Error: invalid command\n");
					printf("Enter your command:\n");
				}
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
			  exitGame(userBoard);
		  }

	  }

	  /*NEED TO EXIT*/
	  exitGame(userBoard);

}

