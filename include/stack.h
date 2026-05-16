#ifndef STACK_H
#define STACK_H

#include "data_structures.h"

TStack* toStack(TList *merged);
TStack* getInfoPersonality(TStack *stk, char *name);

TStack* sortNameStack(TStack *s);
TStack* deleteName(TStack *stk, char *name);
TStack* updateStack(TStack *stk, char *name, char *def, char *DoB, char *DoD);

TQueue* stackToQueue(TStack *stk);
TList* stackToList(TStack *stk);

#endif 
