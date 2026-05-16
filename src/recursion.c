#include "../include/data_structures.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// all function in this file work with recursion
int countOccurence(FILE *f, char *name){
    char line[256];
    if (fgets(line, sizeof(line), f) == NULL){
        return 0;
    }
    char temp[256];
    strcpy(temp, line);

    char *eq = strchr(temp, '=');
    if (eq) {
        *eq = '\0';
    if (strcmp(temp, name) == 0){
        return 1 + countOccurence(f, name);
    }
}
    return countOccurence(f, name);

}
FILE* removeOccurrence(FILE *f, FILE *tmp, char *word) {
    char line[256];
    if (fgets(line, sizeof(line), f) == NULL) {
        return tmp;
    }
    char temp[256];
    strcpy(temp, line);

    char *eq = strchr(temp, '=');
    if (eq) {
        *eq = '\0';
    if (strcmp(temp, word) == 0) {
       fputs(line, tmp);
    }
    }
    return removeOccurrence(f, tmp, word);
}
FILE* replaceOccurence(FILE *f, FILE *tmp, char *name, char *definition, char *DoB, char *DoD) {

    char line[256];

    if (fgets(line, sizeof(line), f) == NULL) {
        return tmp;
    }

    char temp[256];
    strcpy(temp, line);

    char *eq = strchr(temp, '=');

    if (eq) {
        *eq = '\0';

        if (strcmp(temp, name) == 0) {
            fprintf(tmp,"%s=%s{%s}{%s}\n",name,definition,DoB,DoD);
        } else {
            fputs(line, tmp);
        }
    }

    return replaceOccurence(f, tmp, name, definition, DoB, DoD);
}
void swap(char *a, char *b) {
    char temp = *a;
    *a = *b;
    *b = temp;
}
void namePermutation(char *name, int start, int end) {
  int i;
  if (start == end) {
    printf("%s\n", name);
  } else {
    for (i = start; i <= end; i++) {
      swap(&name[start], &name[i]);
      namePermutation(name, start + 1, end);
      swap(&name[start], &name[i]); 
    }
  }
}
static date *convert(char str[]) {
  date *d = malloc(sizeof(date));
  if (!d)
    return NULL;
  if (sscanf(str, "%d/%d/%d", &d->day, &d->month, &d->year) != 3) {
    free(d);
    return NULL;
  }
  return d;
}
// --------------------------------------------------------
// this function built by ai so i dont understand it 
void printSub(char *word, char *res, int i, int j) {
    if (word[i] == '\0') {
        res[j] = '\0';
        if (j > 0) printf("%s\n", res);
        return;
    }
    // Option 1: skip the character
    printSub(word, res, i + 1, j);
    
    // Option 2: take the character
    res[j] = word[i];
    printSub(word, res, i + 1, j + 1);
}

// this recursive procedure generates all possible subsequences of a given event
void subseqName(char *word) {
    char res[256]; // Simple fixed-size array instead of malloc
    printSub(word, res, 0, 0);
}
void longestSubyear(FILE *f, char *date1, char *date2) {
    char line[512];
    if (fgets(line, sizeof(line), f) == NULL) return;
    
    date *d1 = convert(date1);
    date *d2 = convert(date2);
    
    if (d1 && d2) {
        int v1 = d1->year * 10000 + d1->month * 100 + d1->day;
        int v2 = d2->year * 10000 + d2->month * 100 + d2->day;
        if (v1 > v2) { int tmp = v1; v1 = v2; v2 = tmp; }
        
        char temp[512];
        strcpy(temp, line);
        char *eq = strchr(temp, '=');
        
        if (eq) {
            *eq = '\0';
            char *date_str = eq + 2;
            
            char *brace = strchr(date_str, '}');
            if (brace) {
                *brace = '\0';
            }
            
            date *d3 = convert(date_str);
            
            if (d3) {
                int v3 = d3->year * 10000 + d3->month * 100 + d3->day;
                if (v3 >= v1 && v3 <= v2) {
                    printf("%s\n", temp);
                }
                free(d3);
            }
        }
    }
    free(d1); 
    free(d2);
    longestSubyear(f, date1, date2);
}
int distinctSubseqWord(char *event){
    if(event == NULL) return 0;
    if(*event == '\0') return 0;
    return 2 * distinctSubseqWord(event + 1) + 1;
}
bool isPalindromeWord(char *event ,int i,int j){
    if(i>j) return true;
    if(event[i]!=event[j]) return false;
    return isPalindromeWord(event,i+1,j-1);
}

