#ifndef TOOLS_H_
#define TOOLS_H_

/*
 * Tools Module
 *
 *  This module is in charge of reading from files and writing to files. has save and load methods.
 */

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
int save (Board* board, char *path, int gameMode);

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
int load (char *path, Board** board, int mode);

#endif /* TOOLS_H_ */
