#include <stdio.h>
#include <stdlib.h>
#include "SPBufferset.h"
#include <string.h>
#include "game.h"
#include "undoList.h"

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
	newNode->prev = NULL;
	newNode->next = NULL;

	newList->current = newNode;
	return newList;
}

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

void clearBeyond(Node* cur)
{
	Node* tempPointer;

	if (cur->next != NULL)
	{
		cur = cur->next;
		while (cur->next != NULL)
		{
			tempPointer=cur;
			cur = cur->next;
			/* free the data */
			destroyNode(tempPointer);
		}
		destroyNode(cur);
	}
}

/* assuming that we already erased all the forward nodes
 * and we got a pointer to the previous node.
 */
void destroyNode(Node* newNode)
{
	int movesNum;
	int i;
	movesNum = newNode->movesNum;
	for(i=0;i<movesNum;i++)
	{
		free(newNode->moves[i]);
	}
	free(newNode->moves);
	newNode->next=NULL;
	newNode->prev=NULL;
	free(newNode);
}

/* clears an entire list from memory */
void destroyList(List* undoList)
{
	if(undoList){
		if(undoList->current)
		{
			/* go to the beginning of the list */
			while (undoList->current->prev != NULL)
				{
					undoList->current = undoList->current->prev;
				}
			/*clear from mem. every node but the first one*/
			clearBeyond(undoList->current);
			/* also clear the first node */
			destroyNode(undoList->current);
		}

		/* lastly, clear the list itself */
		free(undoList);
	}
}



