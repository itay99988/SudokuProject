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

List* initList();
void insertSingleMove(int** moves, int moveNum, int x, int y, int prevValue, int z);
void updateMovesInNode(Node** newNode,int** moves, int movesNum);
void addMove(List* undoList, Node* newNode);
void destroyList(List* undoList);

#endif /* UNDOLIST_H_ */
