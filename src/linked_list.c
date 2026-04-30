#include "../include/data_structures.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

TList *createNode(char *name, char *definition, char *DoB, char *DoD) {
  TList *node = (TList *)malloc(sizeof(TList));
  if (node == NULL) {
    printf("Error: Could not allocate memory for the node.\n");
    return NULL;
  }
  if (name)
    strcpy(node->name, name);
  if (definition)
    strcpy(node->definition, definition);
  if (DoB)
    strcpy(node->DoB, DoB);
  if (DoD)
    strcpy(node->DoD, DoD);
  node->next = NULL;
  node->prev = NULL;
  return node;
}
TList *insertAtHead(TList *head, char *name, char *definition, char *DoB,
                    char *DoD) {
  TList *node = createNode(name, definition, DoB, DoD);
  if (node == NULL) {
    return NULL;
  }
  node->next = head;
  if (head != NULL) {
    head->prev = node;
  }
  return node;
}
TList *insertAtTail(TList *head, char *name, char *definition, char *DoB,
                    char *DoD) {
  TList *node = createNode(name, definition, DoB, DoD);
  if (node == NULL) {
    return NULL;
  }
  if (head == NULL) {
    return node;
  }
  TList *current = head;
  while (current->next != NULL) {
    current = current->next;
  }
  current->next = node;
  node->prev = current;
  return head;
}
void printList(TList *head) {
  TList *current = head;
  while (current != NULL) {
    printf("%s\n", current->name);
    current = current->next;
  }
}
void deleteNode(TList *head, char *name) {
  TList *current = head;
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

void freeList(TList *head) {
  TList *current = head;
  while (current != NULL) {
    TList *temp = current;
    current = current->next;
    free(temp);
  }
}
TList *getPersonality(FILE *f) {
  TList *head = NULL;
  char line[256];
  while (fgets(line, sizeof(line), f) != NULL) {
    line[strcspn(line, "\n")] = 0;
    char *name = strtok(line, "=");
    strtok(NULL, "{");
    char *definition = strtok(NULL, "}");
    if (!name || !definition)
      continue;
    head = insertAtTail(head, name, definition, NULL, NULL);
  }
  return head;
}
TList *getDatePersonality(FILE *f) {
  TList *head = NULL;
  char line[256];
  while (fgets(line, sizeof(line), f) != NULL) {
    line[strcspn(line, "\n")] = 0;
    char *name = strtok(line, "=");
    strtok(NULL, "{");
    char *DoB = strtok(NULL, "}");
    strtok(NULL, "{");
    char *DoD = strtok(NULL, "}");
    if (!name || !DoB || !DoD)
      continue;
    head = insertAtTail(head, name, NULL, DoB, DoD);
  }
  return head;
}
void getInfoByDates(TList *s, char DoB[]) {
  TList *current = s;
  while (current != NULL) {
    if (strcmp(current->DoB, DoB) == 0) {
      printf("Name: %s\n", current->name);
      printf("DoB: %s\n", current->DoB);
      printf("DoD: %s\n", current->DoD);
    }
    current = current->next;
  }
}
void getInfoByDates2(TList *s, char DoD[]) {
  TList *current = s;
  while (current != NULL) {
    if (strcmp(current->DoD, DoD) == 0) {
      printf("Name: %s\n", current->name);
      printf("DoB: %s\n", current->DoB);
      printf("DoD: %s\n", current->DoD);
    }
    current = current->next;
  }
}
TList *sortWord(TList *syn) {
  if (syn == NULL || syn->next == NULL) {
    return syn;
  }
  TList *i, *j;
  for (i = syn; i->next != NULL; i = i->next) {
    for (j = i->next; j != NULL; j = j->next) {
      if (strcmp(i->name, j->name) > 0) {
        char temp[100];
        strcpy(temp, i->name);
        strcpy(i->name, j->name);
        strcpy(j->name, temp);
        strcpy(temp, i->definition);
        strcpy(i->definition, j->definition);
        strcpy(j->definition, temp);
        strcpy(temp, i->DoB);
        strcpy(i->DoB, j->DoB);
        strcpy(j->DoB, temp);
        strcpy(temp, i->DoD);
        strcpy(i->DoD, j->DoD);
        strcpy(j->DoD, temp);
      }
    }
  }
  return syn;
}
TList *sortWord2(TList *syn) {
  if (syn == NULL || syn->next == NULL) {
    return syn;
  }
  TList *i, *j;
  for (i = syn; i->next != NULL; i = i->next) {
    for (j = i->next; j != NULL; j = j->next) {
      if (strlen(i->name) > strlen(j->name)) {
        char tmp[100];
        strcpy(tmp, i->name);
        strcpy(i->name, j->name);
        strcpy(j->name, tmp);
        strcpy(tmp, i->definition);
        strcpy(i->definition, j->definition);
        strcpy(j->definition, tmp);
        strcpy(tmp, i->DoB);
        strcpy(i->DoB, j->DoB);
        strcpy(j->DoB, tmp);
        strcpy(tmp, i->DoD);
        strcpy(i->DoD, j->DoD);
        strcpy(j->DoD, tmp);
      }
    }
  }
  return syn;
}
date *convert(char str[]) {
  date *d = malloc(sizeof(date));
  if (!d)
    return NULL;
  if (sscanf(str, "%d/%d/%d", &d->day, &d->month, &d->year) != 3) {
    free(d);
    return NULL;
  }
  return d;
}
int agecalculate(date *start, date *end) {
  if (end == NULL)
    return 2025 - start->year;
  return end->year - start->year;
}

TList *sortPersonality(TList *syn) {
  if (syn == NULL || syn->next == NULL) {
    return syn;
  }
  TList *i, *j;
  for (i = syn; i->next != NULL; i = i->next) {
    for (j = i->next; j != NULL; j = j->next) {
      if (agecalculate(convert(i->DoB), convert(i->DoD)) >
          agecalculate(convert(j->DoB), convert(j->DoD))) {
        char temp[100];
        strcpy(temp, i->name);
        strcpy(i->name, j->name);
        strcpy(j->name, temp);
        strcpy(temp, i->definition);
        strcpy(i->definition, j->definition);
        strcpy(j->definition, temp);
        strcpy(temp, i->DoB);
        strcpy(i->DoB, j->DoB);
        strcpy(j->DoB, temp);
        strcpy(temp, i->DoD);
        strcpy(i->DoD, j->DoD);
        strcpy(j->DoD, temp);
      }
    }
  }
  return syn;
}