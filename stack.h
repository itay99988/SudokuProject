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

/*
 * initStack
 *
 *  This function initializes new stack structure, with no nodes
 *  @return - pointer to the new stack
 */
Stack* initStack();

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
void push(Stack* stack, int i, int j, int k);

/*
 * pop
 *
 *  The function pops the top node from the stack and make the poppedNode pointer to point on it
 *  Please note that the allocated memory for the popped node happens *outside* of this function.
 *  @param stack - pointer to the current stack
 *  @param poppedNode - pointer to the node which is going to be the popped node
 *  @return -
 */
void pop(Stack* stack, StackNode* poppedNode);

/*
 * top
 *
 *  This function just returns a pointer to the top node of the stack. (the only node which is
 *  directly accessible). The top function is for viewing purpose only
 *  @param stack - pointer to the current stack
 *  @return pointer to the top node of the stack.
 */
StackNode* top(Stack* stack);

/*
 * isEmpty
 *
 *  This function checks whether or not the stack is empty.
 *  @param stack - pointer to the current stack
 *  @return 1 if empty, 0 if not.
 */
int isEmpty(Stack* stack);

/*
 * destroyStack
 *
 *  This function clears the stack from memory.
 *  @param stack - pointer to the current stack
 *  @return -
 */
void destroyStack(Stack* stack);

#endif /* STACK_H_ */
