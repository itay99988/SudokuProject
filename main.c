/*
 * Main Module
 *
 *  This module is in charge of calling the the startGame function in game.c
 */

#include <stdio.h>
#include <stdlib.h>
#include "SPBufferset.h"
#include <string.h>
#include <time.h>
#include "game.h"

int main(int argc, char *argv[]){
	int seed;
	if(argc < 2)
		seed = 0;
	else
		seed = atoi(argv[1]);
	SP_BUFF_SET();
	srand(seed);
	return startGame(3,3);
}
