#ifndef UNDOLIST_H_
#define UNDOLIST_H_

typedef struct Node {
    int **moves;
    int movesNum;
    struct Node* next; /* Pointer to next node in DLL */
    struct Node* prev; /* Pointer to previous node in DLL */
} Node;

typedef struct List {
	Node* current;
} List;

List* initList();
void insertSingleMove(int** moves, int moveNum, int x, int y, int prevValue, int z);
void updateMovesInNode(Node** newNode,int** moves, int movesNum);
void addMove(List* undoList, Node* newNode);
void destroyList(List* undoList);

#endif /* UNDOLIST_H_ */
