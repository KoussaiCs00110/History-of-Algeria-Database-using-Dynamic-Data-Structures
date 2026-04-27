#ifndef TYPES_H
#define TYPES_H
 // ========================library : 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// =========================contant : 
#define MAX_NAME   100
#define MAX_DEF    500
#define MAX_DATE   15
// =========================structs : 
typedef struct {
    int day;
    int month;
    int year;
} date;
// linked list : 
typedef struct Tlist {
      char name[MAX_NAME];
      char def[MAX_DEF];
      date DoB;
      date DoD;
      struct Tlist *next;
      struct Tlist *prev;
}Tlist;
typedef struct {
      Tlist *head ;
      Tlist *tail ;
}list;
// queue : 
typedef struct TQueue{
      char name[MAX_NAME];
      char def[MAX_DEF];
      date DoB;
      date DoD;
      struct TQueue *next;
}TQueue;
typedef struct{
      TQueue *head;
      TQueue *tail;
}Queue;
// stuck : 
typedef struct Tstack {
      char name[MAX_NAME];
      char def[MAX_DEF];
      date DoB;
      date DoD;
      struct Tstack *next;
}Tstack;
typedef struct {
      Tstack *top;
}stuck;


























#endif 