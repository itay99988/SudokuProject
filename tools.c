/*
 * Tools Module
 *
 *  This module is in charge of reading from files and writing to files. has save and load methods.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "game.h"
#include "solver.h"

/* Public methods: */

/*
 * save
 *
 *  This function gets the game board and a path and saves if possible.
 *
 *  @param board - the actual game board
 *  @param path - a string of the path
 *  @param gameMode - current game mode
 *  @return - 1 if save succeeded, 0 if not.
 */

int save (Board* board, char *path, int gameMode)
{
	int i, j;
	int n,m,size;
	FILE *f = fopen(path, "w");

	/*definitions of dimentions: */
	n=board->n, m=board->m, size=board->boardsize;

	if (f == NULL)
	{
		printf("Error: File cannot be created or modified\n");
		return 0;
	}
	else
	{
		fprintf(f, "%d %d\n", m, n);
		for(i=0;i<size;i++)
		{
			for(j=0;j<size;j++)
			{
				fprintf(f,"%d",board->cells[i][j].value);
				/* prints '.' for fixed cells - if edit mode - fix the values */
				if (board->cells[i][j].fixed==1 || (gameMode==2 && board->cells[i][j].value!=0))
					fprintf(f,".");
				/*
				else
					fprintf(f," ");
				*/
				fprintf(f, " ");
			}
			fprintf(f,"\n");
		}
		printf("Saved to: %s\n", path);
	}
	fclose(f);
	return 1;

}

/*
 * load
 *
 *  This function gets a path and loads a board from this path if possible.
 *
 *  @param board - a pointer to a board
 *  @param path - a string of the path
 *  @param mode - current game mode
 *  @return - 1 if save succeeded, 0 if not.
 */
int load (char *path, Board** board, int mode)
{
	int size, i=0, j=0, m, n, firstCycle=1;
	FILE *f = fopen(path, "r");
    char line[2048];
    char * data;
    char delimiters[] = " \t\r\n";
    i = 0, j = 0;

	if (f == NULL)
	{
	    printf("Error: File doesn't exist or cannot be opened!\n");
	    return 0;
	}
	else
	{
		if(fgets(line, sizeof line, f) != NULL) /* reads from file line by line */
		{
			/* first - initiate the board according to the size */
			m = (strtok(line,delimiters))[0]-'0';
			n = (strtok(NULL,delimiters))[0]-'0';
			/*printf("m=%d, n=%d\n",m,n);*/
			size = m*n;
			*board = init(n,m);

			do {
				/*printf("%s\n",line);*/
				/*need to get the values from the line*/
				/* skip the m and n parameters if this is the first line */
				if (firstCycle)
				{
					data = strtok(NULL,delimiters);
					firstCycle = 0;
				}
				else
					data = strtok(line,delimiters);

				while ((i+1)*(j+1)<=size*size && data!=NULL)
				{
					/*printf("%s ",data);*/
					if (data[strlen(data)-1] == '.')
					{
						if(mode==1) /*solve MODE*/
							(*board)->cells[i][j].fixed = 1;
						else /*mode==2 ---> EDIT MODE*/
							(*board)->cells[i][j].fixed = 0;
						data[strlen(data)-1] = '\0';
					}

					(*board)->cells[i][j].value = atoi(data);
					/*printf("\n");*/

					data = strtok(NULL,delimiters); /*skip a delimiter*/
					/* instead of for loop, calculate the i,j values "handly" */
					if (j<size-1)
					{ j++; }
					else
					{ i++; j=0; }
				}
			}  while(fgets(line, sizeof line, f) != NULL);
		}
	}

	/* mark errors in the board */
	markAllBoardErrors(*board);
	fclose(f);
	return 1;
}

/* End of public methods */
