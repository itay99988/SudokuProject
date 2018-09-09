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
	/* Game mode: 0 - Init, 1 - Solve, 2 - Edit */
	int mode = 0, i = 0, currentMarkErrors = 1, isCommandValid=1; /* mode - starts in Init mode */
	char input[2048], *string[256], delimiters[] = " \t\r\n";
	List* undoList = NULL;
	Board* userBoard = NULL;
	input[256] = 'y'; input[257] = '@'; input[258] = 't'; input[259]='a'; input[260]='m';
	/*const int initialBoardDimension = 3;  according to the forum  is 3*3 - want to change it to define???, was moved to game.c*/
	printf("Enter your command:\n");
	if(fgets(input, sizeof input, stdin)!=NULL) {
		if (input[256]=='y' && input[257]=='@' && input[258]=='t' && input[259]=='a' && input[260]=='m'){
			string[i]=strtok(input,delimiters);
			while(string[i]!=NULL){
			  i++;
			  string[i]=strtok(NULL,delimiters);
			}
		}
		else{
			printf("Error: invalid command\n");
			isCommandValid = 0;
		}
	}
	else { exit(0); /*GOT EOF, therefore exits*/ }

	while(1)
	{
		if(string[0]!='\0' && isCommandValid){
			/*if ((strcmp(string[0],"set")==0) && string[1]!=NULL && isInt(string[1]) && string[2]!=NULL && isInt(string[2]) && string[3]!=NULL && isInt(string[3]) && (mode==1 || mode==2))*/ /*available in solve or edit*/
			if ((strcmp(string[0],"set")==0) && string[1]!=NULL && string[2]!=NULL && string[3]!=NULL && (mode==1 || mode==2)) /*available in solve or edit*/
				{ doSet(userBoard,undoList, string[1], string[2], string[3],&mode); }
			/*else if (strcmp(string[0],"hint")==0 && string[1]!=NULL && isInt(string[1]) && string[2]!=NULL && isInt(string[2]) && mode==1)*/ /*available only in solve*/
			else if (strcmp(string[0],"hint")==0 && string[1]!=NULL && string[2]!=NULL && mode==1) /*available only in solve*/
				{ doHint(userBoard, string[1],string[2]); }
			else if (strcmp(string[0],"validate")==0 && (mode==1 || mode==2))/*available in solve or edit*/
				{ doValidate(userBoard); }
			else if (strcmp(string[0],"reset")==0 && (mode==1 || mode==2))/*available in solve or edit*/
				{ reset(userBoard,&undoList); }
			else if (strcmp(string[0],"solve")==0 && string[1]!=NULL) /*available in every mode*/
				{ doSolve(string[1], &userBoard, &undoList, &mode, currentMarkErrors); }
			else if (strcmp(string[0],"edit")==0) /*available in every mode*/
				{ doEdit(string[1],&userBoard, &undoList, &mode); }
			else if (strcmp(string[0],"mark_errors")==0 && string[1]!=NULL && mode==1) /*available only in solve*/
				{ doMarkErrors(userBoard, string[1], &currentMarkErrors); }
			else if (strcmp(string[0],"print_board")==0 && (mode==1 || mode==2)) /*available in solve or edit*/
				{ printBoard(userBoard); }
			else if (strcmp(string[0],"generate")==0 && string[1]!=NULL && string[2]!=NULL && mode==2) /*available only in edit*/
				{ doGenerate(userBoard, undoList, string[1], string[2]); }
			else if (strcmp(string[0],"undo")==0 && (mode==1 || mode==2)) /*available in solve or edit*/
				{ doUndo(userBoard,undoList,1, &mode); }
			else if (strcmp(string[0],"redo")==0 && (mode==1 || mode==2)) /*available in solve or edit*/
				{ redo(userBoard,undoList,1, &mode); }
			else if (strcmp(string[0],"save")==0 && string[1]!=NULL && (mode==1 || mode==2)) /*available in solve or edit*/
				{ doSave(userBoard, string[1], mode); }
			else if (strcmp(string[0],"num_solutions")==0 && (mode==1 || mode==2))/*available in solve or edit*/
				{ doNumSolutions(userBoard); }
			else if ((strcmp(string[0],"autofill")==0) && (mode==1)) /*available only in solve*/
				{ doAutoFill(userBoard, undoList, &mode); }
			else if (strcmp(string[0],"exit")==0) /*available in every mode*/
				{ exitGame(userBoard, undoList); }
			else { printf("Error: invalid command\n"); }

			printf("Enter your command:\n");
		}
		else { printf("Enter your command:\n"); }

		i=0; input[256] = 'y'; input[257] = '@'; input[258] = 't'; input[259]='a'; input[260]='m'; isCommandValid = 1;
		if(fgets(input, sizeof input, stdin)!=NULL){
			if (input[256]=='y' && input[257]=='@' && input[258]=='t' && input[259]=='a' && input[260]=='m'){
				string[i]=strtok(input,delimiters);
				while(string[i]!=NULL){
				  i++;
				  string[i]=strtok(NULL,delimiters);
				}
			}
			else{
				printf("Error: invalid command\n");
				isCommandValid = 0;
			}
		}
		else{ exitGame(userBoard, undoList); /*GOT EOF, therefore exits*/ }
	}
	exitGame(userBoard, undoList); 	/*NEED TO EXIT*/
}

/* End of public methods */
