#ifndef STACK_H
#define STACK_H

#include "data_structures.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ── Section 3 - Stack Modules ── */
TStack* toStack(TList *merged);
TStack* getInfoPersonality(TStack *stk, char *name);
TStack* sortNameStack(TStack *s);
TStack* deleteName(TStack *stk, char *name);
TStack* updateStack(TStack *stk, char *name, char *def, char *DoB, char *DoD);
TQueue* stackToQueue(TStack *stk);
TList*  stackToList(TStack *stk);
TStack* addNameStack(TStack *stk, char *name, char *definition, char *DoB, char *DoD);
TStack* definitionStack(TStack *stk);
void    sortEventStack(TStack *stk);
void    pronunciationStack(TStack *stk, TStack **shortStack, TStack **longStack);
char*   getSmallest(TStack *stk);
void    continuousSearch(TStack *stk);
bool    isPersonalityKilled(char *word);
TStack* recRevStack(TStack *stk);

#endif
