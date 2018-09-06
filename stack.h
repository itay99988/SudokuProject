/*
 * Stack Module
 *
 *  This module describes the stack data structure which we are using in order to simulate
 *  recursive calls.
 *  The functions here are directly related to the structure.
 *  Memory management of the stack is also done here
 */

#ifndef STACK_H_
#define STACK_H_

/* The stack node struct: describes the smallest unit of the stack
 * each recursion depth level is represented as a node in the stack */
typedef struct node {
    int column;
    int row;
    int value;
    struct node* prev; /*pointer to the previous element */
} StackNode;

/* The stack structure itself: has a pointer the top stackNode, and also has a length field*/
typedef struct stack {
	StackNode* currentNode;
	int length;
} Stack;

Stack* initStack();
void push(Stack* stack, int i, int j, int k);
void pop(Stack* stack, StackNode* poppedNode);
StackNode* top(Stack* stack);
int isEmpty(Stack* stack);
void destroyStack(Stack* stack);

#endif /* STACK_H_ */
