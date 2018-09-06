/*
 * Stack Module
 *
 *  This module describes the stack data structure which we are using in order to simulate
 *  recursive calls.
 *  The functions here are directly related to the structure.
 *  Memory management of the stack is also done here
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stack.h"

/* Public methods: */

/*
 * initStack
 *
 *  This function initializes new stack structure, with no nodes
 *  @return - pointer to the new stack
 */
Stack* initStack() {
	/* memory allocation for the new stack */
    Stack* newStack = malloc(sizeof(Stack));
    if (newStack == NULL) {
		exit(0);
	}
    newStack->currentNode = NULL; /*no nodes in the stack*/
    newStack->length=0;

    return newStack;
}

/*
 * push
 *
 *  The function pushes a new node to the stack and update its length
 *  @param stack - pointer to the current stack
 *  @param i - row of the currently simulated recursive run
 *  @param j - column of the currently simulated recursive run
 *  @param k - value of the currently simulated recursive run
 *  @return -
 */
void push(Stack* stack, int i, int j, int k) {
    StackNode* tmp = malloc(sizeof (StackNode));
    if (tmp == NULL) {
        exit(0);
    }
    /* assignment of values to new node */
    tmp->column = i;
    tmp->row = j;
    tmp->value = k;
    tmp->prev = stack->currentNode;
    /* set the new node as a stacknode */
    stack->currentNode = tmp;
    stack->length = stack->length + 1;
}

/*
 * pop
 *
 *  The function pops the top node from the stack and make the poppedNode pointer to point on it
 *  Please note that the allocated memory for the popped node happens *outside* of this function.
 *  @param stack - pointer to the current stack
 *  @param poppedNode - pointer to the node which is going to be the popped node
 *  @return -
 */
void pop(Stack* stack, StackNode* poppedNode) {
	/* the memory for poppedNode has already been allocated */
    StackNode* tmp = stack->currentNode;
    poppedNode->column = tmp->column;
    poppedNode->row = tmp->row;
    poppedNode->value = tmp->value;

    /* delete the top node from stack */
    stack->currentNode = stack->currentNode->prev;
    stack->length = stack->length - 1;
    /* free the popped node from memory */
    free(tmp);
}

/*
 * top
 *
 *  This function just returns a pointer to the top node of the stack. (the only node which is
 *  directly accessible). The top function is for viewing purpose only
 *  @param stack - pointer to the current stack
 *  @return pointer to the top node of the stack.
 */

StackNode* top(Stack* stack) {
    return stack->currentNode;
}

/*
 * isEmpty
 *
 *  This function checks whether or not the stack is empty.
 *  @param stack - pointer to the current stack
 *  @return 1 if empty, 0 if not.
 */
int isEmpty(Stack* stack) {
    return (stack->length == 0);
}

/*
 * destroyStack
 *
 *  This function clears the stack from memory.
 *  @param stack - pointer to the current stack
 *  @return -
 */
void destroyStack(Stack* stack) {
	free(stack);
}

/* End of public methods */
