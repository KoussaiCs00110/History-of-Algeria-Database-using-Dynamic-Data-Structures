#include <stdio.h>
#include <stdlib.h>
#include "../include/data_structures.h"

int main() {
    printf("History of Algeria Database using Dynamic Data Structures\n");
    printf("\n\n\\n");

    FILE *file = fopen("data/database.txt", "r");
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
