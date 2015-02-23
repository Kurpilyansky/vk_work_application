#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>

#include "buffered_reader.h"
#include "hash_table.h"

#define MAX_SIZE (128 * 1024 * 1024)
char text[MAX_SIZE + 1];

void indexLinesInFile(const char * filename, struct HashTable* hashTablePtr) {
    FILE* file = fopen(filename, "r");
    assert(file != NULL);

    struct BufferedReader reader;
    init_BufferedReader(&reader, file);

    size_t offset = 0;
    char* s;
    while (readLine(&reader, s = text + offset, sizeof(text) - 1 - offset)) {
        size_t length = strlen(s);
        if (tryAddIntoHashTable(hashTablePtr, s)) {
            offset += length + 1;
        }
    }

    fclose(file);
}

int main(int argc, char** argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    struct HashTable hashTable;
    init_HashTable(&hashTable, 10);

    indexLinesInFile(argv[1], &hashTable);

    struct BufferedReader reader;
    init_BufferedReader(&reader, stdin);

    static char input[MAX_SIZE + 1];
    while (readLine(&reader, input, sizeof(input) - 1)) {
        if (strcmp(input, "exit") == 0) {
            break;
        }
        printf("%s\n", containsIntoHashTable(&hashTable, input) ? "YES" : "NO");
    }

    destruct_HashTable(&hashTable);
	return 0;
}
