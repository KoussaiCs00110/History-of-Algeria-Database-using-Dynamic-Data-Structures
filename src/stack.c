#include "../include/stack.h"
#include "../include/data_structures.h"
#include "../include/linked_list.h"

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
        newNode->event_date[0] = '\0';
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
/* TStack* addNameStack(TStack *stk, char *name, char *definition, char *DoB, char *DoD): this function adds a
   personality name with definition and dates into a sorted stack. */
TStack* addNameStack(TStack *stk, char *name, char *definition, char *DoB, char *DoD){
    TStack *newNode = (TStack*)malloc(sizeof(TStack));
    if(newNode == NULL) return stk;
    strcpy(newNode->name, name);
    strcpy(newNode->definition, definition);
    strcpy(newNode->DoB, DoB);
    strcpy(newNode->DoD, DoD);
    newNode->event_date[0] = '\0';
    return sortedInsert(stk, newNode);    
}
/* countWords is declared in linked_list.h and defined in linked_list.c */
/* TStack* definitionStack(TStack *stk): this function sorts the personality names according to the number of
   words in their definition, where each part is separated by a space. */
TStack* definitionStack(TStack *stk){
    if(stk == NULL) return NULL;
    int swapped;
    TStack *cur;
    do {
        swapped = 0;
        cur = stk;
        while(cur->next != NULL){
            if(countWords(cur->definition) > countWords(cur->next->definition)){
                char tmp_name[100], tmp_def[500], tmp_DoB[20], tmp_DoD[20];
                strcpy(tmp_name, cur->name);
                strcpy(cur->name, cur->next->name);     
                strcpy(cur->next->name, tmp_name);

                strcpy(tmp_def, cur->definition); 
                strcpy(cur->definition, cur->next->definition); 
                strcpy(cur->next->definition, tmp_def);

                strcpy(tmp_DoB, cur->DoB); 
                strcpy(cur->DoB, cur->next->DoB);       
                strcpy(cur->next->DoB, tmp_DoB);
                
                strcpy(tmp_DoD, cur->DoD); 
                strcpy(cur->DoD, cur->next->DoD);      
                strcpy(cur->next->DoD, tmp_DoD);
                swapped = 1;
            }
            cur = cur->next;
        }
    } while(swapped);
    return stk;
}   
// this fuction sort event accoding the number of words in there definition
void sortEventStack(TStack *stk){
    if(stk == NULL) return ;
    int swapped;
    TStack *cur;
    do {
        swapped = 0;
        cur = stk;
        while(cur->next != NULL){
            if(countWords(cur->event_date) > countWords(cur->next->event_date)){
                char tmp_name[100], tmp_def[500], tmp_DoB[20], tmp_DoD[20];
                strcpy(tmp_name, cur->name);
                strcpy(cur->name, cur->next->name);     
                strcpy(cur->next->name, tmp_name);

                strcpy(tmp_def, cur->definition); 
                strcpy(cur->definition, cur->next->definition); 
                strcpy(cur->next->definition, tmp_def);

                strcpy(tmp_DoB, cur->DoB); 
                strcpy(cur->DoB, cur->next->DoB);       
                strcpy(cur->next->DoB, tmp_DoB);

                strcpy(tmp_DoD, cur->DoD); 
                strcpy(cur->DoD, cur->next->DoD);      
                strcpy(cur->next->DoD, tmp_DoD);
                swapped = 1;
            }
            cur = cur->next;
        }
    } while(swapped);
}
/* TStack* pronunciationStack(TStack *stk): this function sorts the events according to how they are described:
   short or long into two stacks. */
void pronunciationStack(TStack *stk, TStack **shortStack, TStack **longStack){
    if(stk == NULL) return;
    TStack *current = stk;
    while(current != NULL){
        TStack *newNode = (TStack*)malloc(sizeof(TStack));
        strcpy(newNode->name, current->name);
        strcpy(newNode->definition, current->definition);
        strcpy(newNode->DoB, current->DoB);
        strcpy(newNode->DoD, current->DoD);
        strcpy(newNode->event_date, current->event_date);
        if(countWords(current->definition) < 10){
            newNode->next = *shortStack;
            *shortStack = newNode;
        } else {
            newNode->next = *longStack;
            *longStack = newNode;
        }
        current = current->next;
    }
    sortEventStack(*shortStack);
    sortEventStack(*longStack);
}
/* char* getSmallest(TStack *stk): this function returns the smallest definition in the stack. */
char* getSmallest(TStack *stk){
    if(stk == NULL) return NULL;
    TStack *current = stk;
    char *smallest = (char*)malloc(sizeof(char)*100);
    strcpy(smallest, current->event_date);
    while(current != NULL){
        if(countWords(current->event_date) < countWords(smallest)){
            strcpy(smallest, current->event_date);
        }
        current = current->next;
    }
    return smallest;        
}
/* stackConvert: parse "DD/MM/YYYY" string into a date value (stack-local helper) */
static date stackConvert(char *str){
    date d;
    d.day = 0; d.month = 0; d.year = 0;
    d.full_date[0] = '\0';
    if(str == NULL || str[0] == '\0') return d;
    sscanf(str, "%d/%d/%d", &d.day, &d.month, &d.year);
    strncpy(d.full_date, str, sizeof(d.full_date) - 1);
    d.full_date[sizeof(d.full_date) - 1] = '\0';
    return d;
}

/* returns 1 if date a == date b, 0 otherwise */
static int compareDates(date a, date b){
    return (a.day == b.day && a.month == b.month && a.year == b.year);
}

/* void continuousSearch(TStack *stk): this procedure prints each continuous events where dates are overlapping. */
void continuousSearch(TStack *stk){
    if(stk == NULL) return;

    TStack *i = stk;
    while(i != NULL){
        date di = stackConvert(i->event_date);

        // check if this date was already processed before i (avoid re-printing)
        int already_printed = 0;
        TStack *prev = stk;
        while(prev != i){
            if(compareDates(stackConvert(prev->event_date), di)){
                already_printed = 1;
                break;
            }
            prev = prev->next;
        }

        if(!already_printed){
            // count how many events share this date
            int count = 0;
            TStack *j = i;
            while(j != NULL){
                if(compareDates(stackConvert(j->event_date), di))
                    count++;
                j = j->next;
            }
            // print group only if more than one event shares this date (overlap)
            if(count > 1){
                printf("Events overlapping on date %02d/%02d/%04d:\n",di.day, di.month, di.year);
                j = i;
                while(j != NULL){
                    if(compareDates(stackConvert(j->event_date), di))
                        printf("  - %s\n", j->name);
                    j = j->next;
                }
                printf("\n");
            }
        }

        i = i->next;
    }
}
/* bool isPersonalityKilled(char *word): this function checks if a personality was killed (if it is mentioned in the
   definition that the personality was killed). */
bool isPersonalityKilled(char *word){
    if(word == NULL) return false;
    char *killed = strstr(word, "killed");
    return killed != NULL;
}
/* recRevStack helper: append node to end of a stack */
static TStack* appendToEnd(TStack *head, TStack *node){
    if(head == NULL){ node->next = NULL; return node; }
    head->next = appendToEnd(head->next, node);
    return head;
}

/* TStack* recRevStack (TStack *stk): this function reverses a stack using recursion. */
TStack* recRevStack(TStack *stk){
    if(stk == NULL) return NULL;
    TStack *newNode = (TStack*)malloc(sizeof(TStack));
    strcpy(newNode->name,stk->name);
    strcpy(newNode->definition,stk->definition);
    strcpy(newNode->DoB,stk->DoB);
    strcpy(newNode->DoD,stk->DoD);
    strcpy(newNode->event_date,stk->event_date);
    newNode->next = NULL;
    TStack *reversedRest = recRevStack(stk->next);
    return appendToEnd(reversedRest, newNode);
}