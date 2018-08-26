/*
 * Main Module
 *
 *  This module is in charge of calling the the startGame function in game.c
 */

/*comment by Yotam: just to see if the commit works*/

#include <stdio.h>
#include <stdlib.h>
#include "SPBufferset.h"
#include <string.h>
#include <time.h>
#include "game.h"

int main(int argc, char *argv[]){
	/* they said there is no arguments */
	int seed;
	if(argc < 2)
		seed = 0;
	else
		seed = atoi(argv[1]);
	SP_BUFF_SET();
	srand(seed);
	startGame();
	return 1;
}
