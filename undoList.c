/*
 * undoList Module
 *
 *  This module describes the doubly linked list structure which we are using in order to handle
 *  undo and redo calls. Every time the board is changed, a new node which describes the change
 *  is added to the structure.
 *  The functions here are directly related to the structure.
 *  Memory management of the stack is also done here.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "undoList.h"

/* Private methods declaration */
void clearBeyond(Node* cur);
void destroyNode(Node* newNode);


/* Public methods: */

/*
 * initList
 *
 *  This function initializes new list structure, with a single dummy node (has a -1 value for
 *  number of moves. this dummy node will be useful for detecting if we got to the beginning
 *  of the list.
 *  @return - pointer to the new list
 */
List* initList()
{
	List* newList;
	Node* newNode;
	newList = malloc(sizeof(List));
	newNode = malloc(sizeof(Node));
	if(!newList || !newNode)
	{
		printf("Error: malloc has failed\n");
		exit(0);
		return NULL;
	}
	/*dummy node preparation*/
	newNode->movesNum = -1;
	newNode->moves = NULL;
	newNode->prev = NULL;
	newNode->next = NULL;

	/*dummy node assignment*/
	newList->current = newNode;
	return newList;
}

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
void insertSingleMove(int** moves, int moveNum, int x, int y, int prevValue, int z){
	int* oneMove = malloc(4*sizeof(int));
	if(!oneMove)
	{
		printf("Error: malloc has failed\n");
		exit(0);
	}

	/* values assignment */
	oneMove[0]=x,oneMove[1]=y,oneMove[2]=prevValue,oneMove[3]=z;
	moves[moveNum] = oneMove;
}

/*
 * updateMovesInNode
 *
 *  insert all moves to a node (represents one user turn)
 *  @param newNode - pointer to the node to be created
 *  @param moves - 2d moves array
 *  @param moveNum - total number of moves in current turn
 *  @return -
 */
void updateMovesInNode(Node** newNode,int** moves, int movesNum){
	*newNode = malloc(sizeof(Node));
	if(!(*newNode))
	{
		printf("Error: malloc has failed\n");
		exit(0);
	}
	/* values assignment */
	(*newNode)->moves = moves;
	(*newNode)->movesNum = movesNum;
	(*newNode)->next = NULL;
	(*newNode)->prev = NULL;
}

/*
 * addMove
 *
 *  The function adds a new node (which was already prepared) to the list
 *  @param undoList - pointer to the current list
 *  @param newNode - pointer to a new node (which was already created and filled)
 *  @return -
 */
void addMove(List* undoList, Node* newNode)
{
	/*we have to clear beyond the current node
	suppose that the clear beyond erases everything after current
	and assigns null in the next field of current
	*/
	clearBeyond(undoList-> current);
	newNode -> next = NULL;
	undoList-> current -> next = newNode;
	newNode -> prev = undoList-> current;
	undoList-> current = undoList-> current -> next;
}

/*
 * destroyList
 *
 *  This function clears the entire list from memory.
 *  @param undoList - pointer to the current list
 *  @return -
 */
void destroyList(List* undoList)
{
	/* if no memory was allocated at all, clear nothing */
	if(undoList){
		if(undoList->current)
		{
			/* go to the beginning of the list */
			while (undoList->current->prev != NULL)
				{
					undoList->current = undoList->current->prev;
				}
			/*clear from memory every node but the first one*/
			clearBeyond(undoList->current);
			/* also clear the first node */
			destroyNode(undoList->current);
		}
		/* lastly, clear the list itself */
		free(undoList);
	}
}

/* End of public methods */


/* Private methods: */

/*
 * clearBeyond
 *
 *  This function clears each node which is placed after the current one.
 *  @param cur - pointer to the current pointed node in the list
 *  @return -
 */
void clearBeyond(Node* cur)
{
	Node* tempPointer;

	/* current pointed node is not the last one in the list */
	if (cur->next != NULL)
	{
		cur = cur->next;
		/* if we haven't got to the last node in the list */
		while (cur->next != NULL)
		{
			/* save current node's pointer */
			tempPointer=cur;
			cur = cur->next;
			/* free the data of the saved pointer and move on to the next one */
			destroyNode(tempPointer);
		}
		destroyNode(cur);
	}
}

/*
 * destroyList
 *
 *  This function clears the entire node from memory. This function is used by the destroy list
 *  function.
 *  @param newNode - pointer to the node to be destroyed
 *  @return -
 */
void destroyNode(Node* newNode)
{
	/* assuming that we already erased all the forward nodes
	 * and we got a pointer to the previous node.
	 */
	int movesNum;
	int i;
	/* if no memory was allocated at all, clear nothing */
	if(newNode){
		movesNum = newNode->movesNum;
		/* clear the 2d moves array */
		for(i=0;i<movesNum;i++){
			if(newNode->moves[i])
				free(newNode->moves[i]);
		}
		if(newNode->moves)
			free(newNode->moves);
		newNode->next=NULL;
		newNode->prev=NULL;
		/* clear the node itself from memory */
		free(newNode);
	}
}

/* End of private methods */
