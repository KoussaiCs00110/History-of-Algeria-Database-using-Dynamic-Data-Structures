#include "../include/data_structures.h"
#include <stdio.h>
#include <stdlib.h>


Tlist* createNode(char *name, char *definition, char *DoB, char *DoD, char *event_date) {
    Tlist* node = (Tlist*) malloc(sizeof(Tlist));
    if (node == NULL) {
        printf("Error: Could not allocate memory for the node.\n");
        return NULL;
    }
    strcpy(node->name, name);
    strcpy(node->definition, definition);
    strcpy(node->DoB, DoB);
    strcpy(node->DoD, DoD);
    strcpy(node->event_date, event_date);
    node->next = NULL;
    node->prev = NULL;
    return node;
}
Tlist* insertAtHead(Tlist* head, char *name, char *definition, char *DoB, char *DoD, char *event_date) {
    Tlist* node = createNode(name, definition, DoB, DoD, event_date);
    if (node == NULL) {
        return NULL;
    }
    node->next = head;
    if (head != NULL) {
        head->prev = node;
    }
    return node;
}
Tlist* insertAtTail(Tlist* head, char *name, char *definition, char *DoB, char *DoD, char *event_date) {
    Tlist* node = createNode(name, definition, DoB, DoD, event_date);
    if (node == NULL) {
        return NULL;
    }
    if (head == NULL) {
        return node;
    }
    Tlist* current = head;
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = node;
    node->prev = current;
    return head;
}
void printList(Tlist* head) {
    Tlist* current = head;
    while (current != NULL) {
        printf("%s\n", current->name);
        current = current->next;
    }
}
void deleteNode(Tlist* head, char *name) {
    Tlist* current = head;
    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            if (current->prev != NULL) {
                current->prev->next = current->next;
            }
            if (current->next != NULL) {
                current->next->prev = current->prev;
            }
            free(current);
            return;
        }
        current = current->next;
    }
}
    

void freeList(Tlist* head) {
    Tlist* current = head;
    while (current != NULL) {
        Tlist* temp = current;
        current = current->next;
        free(temp);
    }
}


