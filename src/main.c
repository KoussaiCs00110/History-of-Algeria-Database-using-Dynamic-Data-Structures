#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "types.h"
#include "linked_list.h"
#include "stack.h"
#include "bst.h"
#include "recursion.h"


int main() {
    printf("History of Algeria Database using Dynamic Data Structures\n");
    printf("\n\n\\n");

    FILE *file = fopen("data/history.txt", "r");
    if (file == NULL) {
        printf("Error: Could not open database file.\n");
        return 1;
    }
    char line[1024];
    while (fgets(line, sizeof(line), file)) {
        printf("%s", line);
    }

    fclose(file);
    return 0;
}
static TList  *personalities = NULL;
static TList  *date_list     = NULL;
static TStack *stack         = NULL;
static TTree  *tree          = NULL;
static char    filepath[512];
