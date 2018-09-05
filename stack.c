#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stack.h"

/* Public methods: */

Stack* initStack() {
    Stack* newStack = malloc(sizeof(Stack));
    if (newStack == NULL) {
		exit(0);
	}
    newStack->currentNode = NULL;
    newStack->length=0;

    return newStack;
}

/* Push a new node to the stack and update if length
 */
void push(Stack* stack, int i, int j, int k) {
    StackNode* tmp = malloc(sizeof (StackNode));
    if (tmp == NULL) {
        exit(0);
    }
    tmp->column = i;
    tmp->row = j;
    tmp->value = k;
    tmp->prev = stack->currentNode;
    stack->currentNode = tmp;
    stack->length = stack->length + 1;
}

/*  The allocated memory for the popped node happens *outside* of this function
 */
void pop(Stack* stack, StackNode* poppedNode) {
    StackNode* tmp = stack->currentNode;
    poppedNode->column = tmp->column;
    poppedNode->row = tmp->row;
    poppedNode->value = tmp->value;
    stack->currentNode = stack->currentNode->prev;
    stack->length = stack->length - 1;
    /* free the popped node from memory */
    free(tmp);
}

/* the top function is for view purpose only */
StackNode* top(Stack* stack) {
    return stack->currentNode;
}

/*
 * Return 1 if empty
 */
int isEmpty(Stack* stack) {
    return (stack->length == 0);
}

/* under the assumption that the stack is empty!! */
void destroyStack(Stack* stack) {
	free(stack);
}

/* End of public methods */
