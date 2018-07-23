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

#define BOARDSIZE 9


/*
 * initialization
 *
 *  This function asks the user for the number of desired cells to fill
 *  @return - number of desired cells to fill, or -1 for EOF
 */
int initialization(){
	int fixedCells;
	printf("Please enter the number of cells to fill [0-80]:\n");
	if(scanf("%d", &fixedCells)==EOF)
	{
		return -1;
		/*
		printf("Exiting...\n");
		exit(0);
		*/
	}

	while (fixedCells<0 || fixedCells>80)
	{
		printf("Error: Invalid number of cells to fill (should be between 0 and 80)\n");
		printf("Please enter the number of cells to fill [0-80]:\n");
		if(scanf("%d", &fixedCells)==EOF)
		{
			return -1;
			/*
			printf("Exiting...\n");
			exit(0);
			*/
		}
	}

	return fixedCells;
}


/*
 * read
 *
 *  This function reads the user's game-plays and interpret them - calls the right function in game
 *  @param generatedBoard - a possible stored solution
 *  @param userBoard - the user's board
 *  @return -
 */
void read(Cell** generatedBoard, Cell** userBoard)
{
	char input[1024];
	char *string[1024];

	char delimiters[] = " \t\r\n";
	int x,y,z;
	int i = 0;
	int solved = 0; /*'set' returns 2 if puzzle was solved*/

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
		  exitGame(generatedBoard, userBoard, BOARDSIZE);
	  }


	  while(string[0]== '\0' || strcmp(string[0],"exit")!=0)
	  {
		  if(string[0]!= '\0')
		  {
			  if (strcmp(string[0],"set")==0 && string[1]!=NULL && string[2]!=NULL && string[3]!=NULL && solved!=2)
			  			{
			  				x = atoi(string[1]);
			  				y = atoi(string[2]);
			  				z = atoi(string[3]);

			  				solved = set(userBoard, y-1,x-1,z);
			  				/*printf("%d %d %d\n",x,y,z);*/
			  			}

			  			else
			  			if (strcmp(string[0],"hint")==0 && string[1]!=NULL && string[2]!=NULL && solved!=2)
			  			{
			  				x = atoi(string[1]);
			  				y = atoi(string[2]);
			  				/*printf("%d %d\n",x,y);*/
			  				hint(generatedBoard, y-1,x-1);
			  			}

			  			else
			  			if (strcmp(string[0],"validate")==0 && solved!=2)
			  			{
			  				/*printf("VALIDATE");*/
			  				validate(generatedBoard,userBoard);
			  			}

			  			else
			  			if (strcmp(string[0],"restart")==0)
			  			{
			  				restart(generatedBoard, userBoard, BOARDSIZE);
			  				/*need to update solved to 0*/
			  				solved = 0;
			  			}

			  			else
			  				printf("Error: invalid command\n");
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
			  exitGame(generatedBoard, userBoard, BOARDSIZE);
		  }

	  }

	  /*NEED TO EXIT*/
	  exitGame(generatedBoard, userBoard, BOARDSIZE);

}

