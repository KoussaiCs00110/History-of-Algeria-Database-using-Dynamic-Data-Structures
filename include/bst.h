#ifndef BST_H
#define BST_H

#include "data_structures.h"

TTree* insertToBST(TTree *tr, char *name, char *definition, char *DoB, char *DoD, char *event_date);
TTree* toTree(TStack *stk);
TTree* fillTree(FILE *f);
TTree* getInfoNameTree(TTree *tr, char *name);
TStack* addNameBST(TTree *tr, char *name, char *DoB, char *DoD);
TTree* deleteNameBST(TTree *tr, char *name);
TTree* updateNameBST(TTree *tr, char *name, char *s, char *DoB, char *DoD);
TTree* traversalBSTinOrder(TTree *tr);
TTree* traversalBSTpreOrder(TTree *tr);
TTree* traversalBSTpostOrder(TTree *tr);
void heightSizeBST(TTree *tr, int *height, int *size);
TTree* lowestCommonAncestor(TTree *tr, char *word1, char *word2);
int countNodesRange(TTree *tr, char *l, char *h);
TTree* inOrderSuccessor(TTree *tr, char *word);
TTree* BSTMirror(TTree *tr);
bool isBalancedBST(TTree *tr);
TTree* BSTMerge(TTree *tr1, TTree *tr2);

#endif // BST_H
