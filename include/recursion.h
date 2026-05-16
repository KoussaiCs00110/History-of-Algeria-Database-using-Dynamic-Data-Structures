#ifndef RECURSION_H
#define RECURSION_H

#include "data_structures.h"
#include <stdbool.h>
#include <stdio.h>

/* ── Section 5 - Recursive file operations ── */
int   countOccurence(FILE *f, char *name);
FILE* removeOccurrence(FILE *f, FILE *tmp, char *word);
FILE* replaceOccurence(FILE *f, FILE *tmp, char *name, char *definition, char *DoB, char *DoD);

/* ── Recursive string operations ── */
void  swap(char *a, char *b);
void  namePermutation(char *name, int start, int end);
void  printSub(char *word, char *res, int i, int j);
void  subseqName(char *word);
int   distinctSubseqWord(char *event);
bool  isPalindromeWord(char *event, int i, int j);

/* ── Recursive date helpers ── */
void  longestSubyear(FILE *f, char *date1, char *date2);

#endif
