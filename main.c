/*
 * Main Module
 *
 *  The following module will be executed first.
 *  This module is in charge of calling the the startGame function in game.c
 */

#include <stdio.h>
#include <stdlib.h>
#include "SPBufferset.h"
#include <time.h>
#include "game.h"

int main(){
	SP_BUFF_SET();
	srand(time(NULL));
	startGame();
	return 0;
}
