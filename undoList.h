/*
 * undoList Module
 *
 *  This module describes the doubly linked list structure which we are using in order to handle
 *  undo and redo calls. Every time the board is changed, a new node which describes the change
 *  is added to the structure.
 *  The functions here are directly related to the structure.
 *  Memory management of the stack is also done here.
 */

#ifndef UNDOLIST_H_
#define UNDOLIST_H_

/* a double-linked list node struct. Each node has a 2D array which describes all the moves that occured
 * in one turn. for example: if the number of moves in one turn is 7, then the dimentions
 * of the array will be 7X4. (4 fields are required in order to describe a single move  */
typedef struct Node {
    int **moves;
    int movesNum;
    struct Node* next; /* Pointer to next node in the DLL */
    struct Node* prev; /* Pointer to previous node in the DLL */
} Node;

/* a doubly-linked list struct */
typedef struct List {
	Node* current; /* this pointer points on the current move in the list */
} List;

/*
 * initList
 *
 *  This function initializes new list structure, with a single dummy node (has a -1 value for
 *  number of moves. this dummy node will be useful for detecting if we got to the beginning
 *  of the list.
 *  @return - pointer to the new list
 */
List* initList();

/*
 * insertSingleMove
 *
 *  insert a single move to a node
 *  @param moves - 2d moves array
 *  @param moveNum - the current move number out of all moves in current turn
 *  @param x - x coordinate
 *  @param y - y coordinate
 *  @param prevValue - value before move
 *  @param z - new value to be assigned

 *  @return -
 */
void insertSingleMove(int** moves, int moveNum, int x, int y, int prevValue, int z);

/*
 * updateMovesInNode
 *
 *  insert all moves to a node (represents one user turn)
 *  @param newNode - pointer to the node to be created
 *  @param moves - 2d moves array
 *  @param moveNum - total number of moves in current turn
 *  @return -
 */
void updateMovesInNode(Node** newNode,int** moves, int movesNum);

/*
 * addMove
 *
 *  The function adds a new node (which was already prepared) to the list
 *  @param undoList - pointer to the current list
 *  @param newNode - pointer to a new node (which was already created and filled)
 *  @return -
 */
void addMove(List* undoList, Node* newNode);

/*
 * destroyList
 *
 *  This function clears the entire list from memory.
 *  @param undoList - pointer to the current list
 *  @return -
 */
void destroyList(List* undoList);

#endif /* UNDOLIST_H_ */
