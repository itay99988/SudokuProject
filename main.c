/*
 * Main Module
 *
 *  The following module will be executed first.
 *  This module is in charge of calling the the startGame function in game.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "game.h"
#include "SPBufferset.h"

/*
 * main
 *
 *  This function is executed first, sets a default random seed
 *  and calls the startGame function in order to start the game
 *  @return 0 (always)
 */
int main(){
	SP_BUFF_SET();
	srand(time(NULL)); /* default seed for randomization */
	startGame();
	return 0;
}
