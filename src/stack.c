#include "../include/stack.h"
#include "../include/data_structures.h"

//TStack* toStack(TList *merged): this function converts the list returned by Merge function into a stack.
TStack* toStack(TList *merged){
    if(merged == NULL)return NULL;
    TStack *stack = NULL;
    TList *current = merged;
    while(current != NULL){
        TStack *newNode = (TStack*)malloc(sizeof(TStack));
        strcpy(newNode->name, current->name);
        strcpy(newNode->definition, current->definition);
        strcpy(newNode->DoB, current->DoB);
        strcpy(newNode->DoD, current->DoD);
        newNode->next = stack;
        stack = newNode;
        current = current->next;
    }
    return stack;    
}
/*TStack* getInfoPersonality(TStack *stk, char *name): this function takes a name as input and returns the
definition, date of birth and date of death from the stack stk.*/
TStack* getInfoPersonality(TStack *stk, char *name){
    if(stk == NULL)return NULL;
    TStack *current = stk;
    while(current != NULL){
        if(strcmp(current->name, name) == 0){
            TStack *result = (TStack*)malloc(sizeof(TStack));
            strcpy(result->name, current->name);
            strcpy(result->definition, current->definition);
            strcpy(result->DoB, current->DoB);
            strcpy(result->DoD, current->DoD);
            strcpy(result->event_date, current->event_date);
            result->next = NULL;
            return result;
        }
        current = current->next;
    }
    return NULL;        
}
//TStack* sortNameStack(TStack *s): this function sorts the stack alphabetically.
static TStack* sortedInsert(TStack *s, TStack *node){
    if(s == NULL || strcmp(node->name, s->name) < 0){
        node->next = s;
        return node;
    }
    s->next = sortedInsert(s->next, node);
    return s;
}

TStack* sortNameStack(TStack *s){
    if(s ==NULL || s->next == NULL) return s;
    TStack *temp = s;
    s = s->next;
    temp->next = NULL;
    s = sortNameStack(s);
    return sortedInsert(s, temp);
}

//TStack* deleteName(TStack *stk, char *name): this function deletes a name from the stack.
TStack* deleteName(TStack *stk, char *name){
    if(stk ==NULL) return NULL;
    if(strcmp(name, stk->name) == 0){
        TStack *temp = stk->next;
        free(stk);
        return temp;
    }
    stk->next = deleteName(stk->next, name);
    return stk;    
}

/*TStack* updateStack(TStack *stk, char *name, char *def, char *DoB, char *DoD): this function updates in the
stack a personality name, definition, date of birth, date of death.*/
TStack* updateStack(TStack *stk, char *name, char *def, char *DoB, char *DoD){
    if(stk ==NULL) return NULL;
    if(strcmp(name, stk->name) == 0){
        strcpy(stk->definition, def);
        strcpy(stk->DoB, DoB);
        strcpy(stk->DoD, DoD);
    }
    stk->next = updateStack(stk->next, name, def, DoB, DoD);
    return stk;    
}
/*TQueue* stackToQueue(TStack *stk): this function converts the stack resulted from the function toStack into a
sorted queue.*/
TQueue* stackToQueue(TStack *stk){
     if(stk == NULL) return NULL;
    TQueue *queue = (TQueue*)malloc(sizeof(TQueue));
    queue->front = NULL;
    queue->rear = NULL;
    TStack *current = stk;
    while(current != NULL){
        NodeQueue *newNode = (NodeQueue*)malloc(sizeof(NodeQueue));
        strcpy(newNode->name, current->name);
        strcpy(newNode->definition, current->definition);
        strcpy(newNode->DoB, current->DoB);
        strcpy(newNode->DoD, current->DoD);
        newNode->next = NULL;
        if(queue->rear == NULL){
            queue->front = newNode;
            queue->rear = newNode;
        }
        else{
            queue->rear->next = newNode;
            queue->rear = newNode;
        }
        current = current->next;
    }
    return queue;        
}

/*TList* stackToList(TStack *stk): this function converts the stack resulted from the function toStack into a
bidirectional sorted linked list.*/
TList* stackToList(TStack *stk){
     if(stk == NULL) return NULL;
    TList *head = NULL;
    TList *tail = NULL;
    TStack *current = stk;
    while(current != NULL){
        TList *newNode = (TList*)malloc(sizeof(TList));
        strcpy(newNode->name, current->name);
        strcpy(newNode->definition, current->definition);
        strcpy(newNode->DoB, current->DoB);
        strcpy(newNode->DoD, current->DoD);
        newNode->next = NULL;
        newNode->prev = tail;
        if(tail != NULL){
            tail->next = newNode;
        }
        else{
            head = newNode;
        }
        tail = newNode;
        current = current->next;
    }
    return head;        
}