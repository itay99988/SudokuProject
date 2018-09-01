/*
 * Tools Module
 *
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


void save (Board* board, char *path, int gameMode)
{
	int i, j;
	int n,m,size;
	FILE *f = fopen(path, "w");

	/*definitions of dimentions: */
	n=board->n, m=board->m, size=board->boardsize;

	if (f == NULL)
	{
		/* printf("Error opening file!\n"); what should we do???*/
		printf("Error: File cannot be created or modified\n");
	}
	else
	{
		fprintf(f, "%d %d\n", m, n);
		for(i=0;i<size;i++)
		{
			for(j=0;j<size;j++)
			{
				fprintf(f,"%d",board->cells[i][j].value);
				if (board->cells[i][j].fixed==1 || (gameMode==2 && board->cells[i][j].value!=0)) /*this is how we implement h*/
					fprintf(f,".");
				else
					fprintf(f," ");
				fprintf(f, " ");
			}
			fprintf(f,"\n");
		}
		printf("Saved to: %s\n", path);
	}
	fclose(f);

}

Board* load (char *path)
{
	int size, i, j, m, n;
	Board *board;
	FILE *f = fopen(path, "r");
    char line[1024];
    char * data;

    char delimiters[] = " \t\r\n";

    i = 0;
    j = 0;
    /*printf("------------------\n");*/

	if (f == NULL)
	{
	    printf("Error opening file!\n"); /*what should we do???*/
	}
	else
	{

		if(fgets(line, sizeof line, f) != NULL)
		{
			/*printf("first line is '%s'\n",line);*/
			m = atoi(strtok(line,delimiters));
			n = atoi(strtok(line,delimiters));
			size = m*n;
			board = init(n,m);
		}

		for (i=0; i<size;i++)
		{
			if(fgets(line, sizeof line, f) != NULL)
			{
				/*printf("%s\n",line);*/
		    	/*need to get the values from the line*/
				data = strtok(line,delimiters);
		    	for (j=0; j<size;j++)
		    	{

		    		/*printf("%s ",data);*/
		    		if (data[strlen(data)-1] == '.')
		    		{
		    			board->cells[i][j].fixed = 1;
		    			data[strlen(data)-1] = '\0';
		    		}

		    		board->cells[i][j].value = atoi(data);
		    		/*printf("\n");*/
		    		data = strtok(NULL,delimiters);
		    	}
			}
	    }
	}

	fclose(f);
	return board;
}
