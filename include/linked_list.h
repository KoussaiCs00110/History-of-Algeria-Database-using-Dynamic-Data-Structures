#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include "data_structures.h"
#include <stdio.h>

// Basic Linked List Operations
TList *createNode(char *name, char *definition, char *DoB, char *DoD);
TList *insertAtHead(TList *head, char *name, char *definition, char *DoB, char *DoD);
TList *insertAtHead2(TList *head, char *name, char *definition, char *DoB, char *DoD);
TList *insertAtTail(TList *head, char *name, char *definition, char *DoB, char *DoD);
void endpointhead(TList *head);
void printList(TList *head);
void deleteNode(TList *head, char *name);
void freeList(TList *head);

// Linked List Project Modules 
TList *getPersonality(FILE *f);
TList *getDatePersonality(FILE *f);
void getInfoByDates(TList *s, char DoB[]);
void getInfoByDates2(TList *s, char DoD[]);
TList *sortWord(TList *syn);
TList *sortWord2(TList *syn);
date *convert(char str[]);
int agecalculate(date *start, date *end);
TList *sortPersonality(TList *syn);
TList *deletepersonality(FILE *f, TList *s, TList *a, char *name);
TList *updatePersonality(FILE *f, TList *s, TList *a, char *name, char *definition, char *DoB, char *DoD);
TList *similarPersonality(TList *s, char *word);
TList *countPersonality(TList *s, date *prt);
int isPalindrome(char str[]);
TList *palindromeName(TList *s);
TList *mergeNodes(TList *s, TList *a);
TList *merge2Nodes(TList *s, TList *a);
TList *addPersonality(TList *s, TList *a, char *name, char *definition, char *DoB, char *DoD);
TList *addEvents(TList *b, char *namEvente, char *date);

// Basic Queue Operations 
NodeQueue *createQNode(char *name, char *definition, char *DoB, char *DoD);
void enqueue(TQueue *queue, char *name, char *definition, char *DoB, char *DoD);
NodeQueue *dequeue(TQueue *queue);
NodeQueue *peek(TQueue *queue);
int isEmpty(TQueue *queue);
void deleteFromQueue(TQueue *queue, char *name);
void freeQueue(TQueue *queue);
void initQueue(TQueue *queue);
int countWords(char *name);

//Queue Project Modules 
TQueue *sName(TList *s);
TQueue *ageP(TList *a);
TQueue *toQueue(TList *merged);

#endif 
