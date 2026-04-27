#ifndef DATA_STRUCTURES_H
#define DATA_STRUCTURES_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>


// 1. Basic Date Structure
// date data stracture : 
typedef struct {
    int day;
    int month;
    int year;
    char full_date[20];
} date;

// 2. Linked List Nodes :
typedef struct NodeList {
    char name[100];
    char definition[500];
    char DoB[20];   
    char DoD[20];   
    char event_date[20];
    
    struct NodeList *next;
    struct NodeList *prev; 
} TList;

// 3. Queue Structure
typedef struct NodeQueue {
    char name[100];
    char definition[500];
    char DoB[20];   
    char DoD[20];   
    char event_date[20];

    struct NodeQueue *next;
} NodeQueue;

typedef struct {
    NodeQueue *front;
    NodeQueue *rear;
} TQueue;

// 4. Stack Structure
typedef struct NodeStack {
    char name[100];
    char definition[500];
    char DoB[20];
    char DoD[20];
    char event_date[20];
    
    struct NodeStack *next;
} TStack;

// 5. Binary Search Tree (BST) Structure
typedef struct TreeNode {
    char name[100];
    char definition[500];
    char DoB[20];
    char DoD[20];
    char event_date[20];

    struct TreeNode *left;
    struct TreeNode *right;
} TTree;

#endif
