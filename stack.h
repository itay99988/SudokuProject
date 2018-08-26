#ifndef STACK_H_
#define STACK_H_

typedef struct node {
    int column;
    int row;
    int value;
    struct node* prev;
} StackNode;

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
