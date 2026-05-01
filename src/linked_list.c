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
TList *insertAtHead2(TList *head, char *name, char *definition, char *DoB,
                     char *DoD) {
  TList *node = createNode(name, definition, DoB, DoD);
  if (node == NULL) {
    return NULL;
  }
  node->next = head;
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
void endpointhead(TList *head) {
  TList *current = head;
  while (current->next != NULL) {
    current = current->next;
  }
  current->next = head;
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
TList *deletepersonality(FILE *f, TList *s, TList *a, char *name) {
  deleteNode(s, name);
  deleteNode(a, name);
  FILE *tmp = fopen("data/tmp.txt", "w");
  char line[256];
  char name2[64];
  while (fgets(line, sizeof(line), f)) {
    strcpy(name2, line);
    char *eq = strstr(name2, "="); // eq is pointer to change = to \0
    if (eq)
      *eq = '\0';
    name2[strcspn(name2, "\n")] = '\0';
    if (strcmp(name, name2) != 0) {
      fputs(line, tmp);
    }
  }
  fclose(tmp);
  fclose(f);
  remove("data/persons.txt");
  rename("data/tmp.txt", "data/persons.txt");
  return s;
}
void updatePersonality(FILE *f, TList *s, TList *a, char *name,
                       char *definition, char *DoB, char *DoD) {}

TList *similarPersonality(TList *s, char *word) {}

TList *countPersonality(TList *s, date *prt) {
  TList *current = s;
  TList *head = NULL;
  while (current != NULL) {
    if (strcmp(current->DoB, prt->full_date) == 0 ||
        strcmp(current->DoD, prt->full_date) == 0) {
      insertAtHead(head, current->name, current->definition, current->DoB,
                   current->DoD);
    }
    current = current->next;
  }
  return head;
}
int isPalindrome(char str[]) {
  int n = strlen(str);
  for (int i = 0; i < n / 2; i++) {
    if (str[i] != str[n - i - 1])
      return 0;
  }
  return 1;
}
TList *palindromeName(TList *s) {
  TList *current = s;
  TList *head = NULL;
  while (current != NULL) {
    if (isPalindrome(current->name) == 1) {
      insertAtHead(head, current->name, current->definition, current->DoB,
                   current->DoD);
    }
    current = current->next;
  }
  return head;
}
TList *mergeNodes(TList *s, TList *a) {
  TList *current = s;
  TList *current2 = a;
  TList *head = NULL;
  while (current != NULL && current2 != NULL &&
         strcmp(current->name, current2->name) > 0 &&
         strlen(current->name) > strlen(current2->name)) {
    insertAtHead(head, current->name, current->definition, current2->DoB,
                 current2->DoD);
    current2 = current2->next;
    current = current->next;
  }
  return head;
}
TList *merge2Nodes(TList *s, TList *a) {
  TList *current = s;
  TList *current2 = a;
  TList *head = NULL;
  while (current != NULL && current2 != NULL &&
         strcmp(current->name, current2->name) > 0 &&
         strlen(current->name) > strlen(current2->name)) {
    insertAtHead2(head, current->name, current->definition, current2->DoB,
                  current2->DoD);
    current2 = current2->next;
    current = current->next;
  }
  endpointhead(head);
  return head;
}
TList *addPersonality(TList *s, TList *a, char *name, char *definition,
                      char *DoB, char *DoD) {
  s = insertAtHead(s, name, definition, DoB, DoD);
  a = insertAtHead(a, name, definition, DoB, DoD);
  FILE *f = fopen("data/history.txt", "a");
  fprintf(f, "%s=%s{%s}{%s}\n", name, definition, DoB, DoD);
  fclose(f);
  return s;
}
TList *addEvents(TList *b, char *namEvente, char *date) {
  b = insertAtTail(b, namEvente, NULL, NULL, date);
  FILE *f = fopen("data/events.txt", "a");
  fprintf(f, "%s=%s{%s}{%s}\n", namEvente, "", "", date);
  fclose(f);
  return b;
}
// queues part :
NodeQueue *createQNode(char *name, char *definition, char *DoB, char *DoD) {
  NodeQueue *node = (NodeQueue *)malloc(sizeof(NodeQueue));
  if (node == NULL)
    return NULL;
  if (name)
    strcpy(node->name, name);
  if (definition)
    strcpy(node->definition, definition);
  if (DoB)
    strcpy(node->DoB, DoB);
  if (DoD)
    strcpy(node->DoD, DoD);
  node->next = NULL;
  return node;
}

void enqueue(TQueue *queue, char *name, char *definition, char *DoB,
             char *DoD) {
  NodeQueue *node = createQNode(name, definition, DoB, DoD);
  if (node == NULL)
    return;
  if (queue->rear == NULL) {
    queue->front = queue->rear = node;
    return;
  }
  queue->rear->next = node;
  queue->rear = node;
}

NodeQueue *dequeue(TQueue *queue) {
  if (queue->front == NULL)
    return NULL;
  NodeQueue *temp = queue->front;
  queue->front = queue->front->next;
  if (queue->front == NULL)
    queue->rear = NULL;
  temp->next = NULL;
  return temp;
}

NodeQueue *peek(TQueue *queue) { return queue->front; }

int isEmpty(TQueue *queue) { return queue->front == NULL; }

void deleteFromQueue(TQueue *queue, char *name) {
  if (queue->front == NULL)
    return;
  NodeQueue *current = queue->front;
  NodeQueue *prev = NULL;
  while (current != NULL) {
    if (strcmp(current->name, name) == 0) {
      if (prev == NULL)
        queue->front = current->next;
      else
        prev->next = current->next;
      if (current->next == NULL)
        queue->rear = prev;
      free(current);
      return;
    }
    prev = current;
    current = current->next;
  }
}

void freeQueue(TQueue *queue) {
  NodeQueue *current = queue->front;
  while (current != NULL) {
    NodeQueue *temp = current;
    current = current->next;
    free(temp);
  }
  queue->front = queue->rear = NULL;
}

void initQueue(TQueue *queue) { queue->front = queue->rear = NULL; }
// counts the number of words in a name
int countWords(char *name) {
  int count = 1;
  for (int i = 0; i < strlen(name); i++) {
    if (name[i] == ' ')
      count++;
  }
  return count;
}

TQueue *sName(TList *s) {
  TQueue *queue = (TQueue *)malloc(sizeof(TQueue));
  if (queue == NULL)
    return NULL;
  initQueue(queue);

  int size = 0;
  TList *current = s;
  while (current != NULL) {
    size++;
    current = current->next;
  }
  if (size == 0)
    return queue;

  TList **arr = (TList **)malloc(size * sizeof(TList *));
  if (arr == NULL)
    return queue;

  current = s;
  for (int i = 0; i < size; i++) {
    arr[i] = current;
    current = current->next;
  }

  for (int i = 0; i < size - 1; i++) {
    for (int j = 0; j < size - i - 1; j++) {
      if (countWords(arr[j]->name) > countWords(arr[j + 1]->name)) {
        TList *temp = arr[j];
        arr[j] = arr[j + 1];
        arr[j + 1] = temp;
      }
    }
  }

  for (int i = 0; i < size; i++) {
    enqueue(queue, arr[i]->name, arr[i]->definition, arr[i]->DoB, arr[i]->DoD);
  }

  free(arr);
  return queue;
}
TQueue *ageP(TList *a) {
  TQueue *queue = (TQueue *)malloc(sizeof(TQueue));
  if (queue == NULL)
    return NULL;
  initQueue(queue);

  int size = 0;
  TList *current = a;
  while (current != NULL) {
    size++;
    current = current->next;
  }
  if (size == 0)
    return queue;

  TList **arr = (TList **)malloc(size * sizeof(TList *));
  if (arr == NULL)
    return queue;

  current = a;
  for (int i = 0; i < size; i++) {
    arr[i] = current;
    current = current->next;
  }

  for (int i = 0; i < size - 1; i++) {
    for (int j = 0; j < size - i - 1; j++) {
      date *dob1 = convert(arr[j]->DoB);
      date *dod1 = convert(arr[j]->DoD);
      date *dob2 = convert(arr[j + 1]->DoB);
      date *dod2 = convert(arr[j + 1]->DoD);

      int age1 = agecalculate(dob1, dod1);
      int age2 = agecalculate(dob2, dod2);

      free(dob1);
      free(dod1);
      free(dob2);
      free(dod2);

      if (age1 > age2) {
        TList *temp = arr[j];
        arr[j] = arr[j + 1];
        arr[j + 1] = temp;
      }
    }
  }

  for (int i = 0; i < size; i++) {
    enqueue(queue, arr[i]->name, arr[i]->definition, arr[i]->DoB, arr[i]->DoD);
  }

  free(arr);
  return queue;
}
TQueue* toQueue(TList *merged){
  TQueue *queue = (TQueue *)malloc(sizeof(TQueue));
  if (queue == NULL)
    return NULL;
  initQueue(queue);
  
  TList *current = merged;
  while (current != NULL) {
    enqueue(queue, current->name, current->definition, current->DoB, current->DoD);
    current = current->next;
  }

  return queue;
}