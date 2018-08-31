#include <stdio.h>
#include <stdlib.h>
#include "SPBufferset.h"
#include <string.h>
#include "game.h"
#include "undoList.h"

List* initList()
{
	List* newList;
	newList = malloc(sizeof(List));
	if(!newList)
	{
		printf("Error: malloc has failed\n");
		exit(0);
		return NULL;
	}
	newList->head = NULL;
	return newList;
}

void addMove(List* undoList, Node* newNode)
{
	if(undoList->head == NULL)
	{
		undoList->head = newNode;
	}
	else
	{
		/*we have to clear beyond the current node
		suppose that the clear beyond erases everything after current
		and assigns null in the next field of current
		*/
		clearBeyond(undoList-> head);
		newNode -> next = NULL;
		undoList-> head -> next = newNode;
		newNode -> prev = undoList-> head;
		undoList-> head = undoList-> head -> next;
	}
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
	if(undoList->head)
	{
		/* go to the beginning of the list */
		while (undoList->head->prev != NULL)
			{
				undoList->head = undoList->head->prev;
			}

		/*clear from mem. every node but the first one*/
		clearBeyond(undoList->head);

		/* also clear the first node */
		destroyNode(undoList->head);
	}

	/* lastly, clear the list itself */
	free(undoList);
}



