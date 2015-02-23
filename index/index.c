#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include <time.h>

#include "buffered_reader.h"
#include "hash_table.h"

void stamp(const char * message) {
    fprintf(stderr, "%s: %.2lf\n", message, ((double)clock()) / CLOCKS_PER_SEC);
    fflush(stderr);
}

#define MAX_SIZE (128 * 1024 * 1024)
char text[MAX_SIZE + 1];

void indexLinesInFile(const char * filename, struct HashTable* hashTablePtr) {
    FILE* file = fopen(filename, "r");
    assert(file != NULL);

    struct BufferedReader reader;
    init_BufferedReader(&reader, file);

    size_t count = 0;
    size_t offset = 0;
    char* s;
    while (readLine(&reader, s = text + offset, sizeof(text) - 1 - offset)) {
        size_t length = strlen(s);
        offset += length + 1;
        ++count;
    }

    fprintf(stderr, "Total count %d\n", (int)count);

    init_HashTableByCapacity(hashTablePtr, count);

    size_t i;
    size_t different_count = 0;
    for (i = 0, offset = 0; i < count; ++i) {
        s = text + offset;
        size_t length = strlen(s);
        if (tryAddIntoHashTable(hashTablePtr, s)) {
            ++different_count;
        }
        offset += length + 1;
    }

    fprintf(stderr, "Different count: %d\n", (int)different_count);

    fclose(file);
}

int main(int argc, char** argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    struct HashTable hashTable;

    indexLinesInFile(argv[1], &hashTable);
    stamp("HashTable has been built");

    struct BufferedReader reader;
    init_BufferedReader(&reader, stdin);

    static char input[MAX_SIZE + 1];
    while (readLine(&reader, input, sizeof(input) - 1)) {
        if (strcmp(input, "exit") == 0) {
            break;
        }
        printf("%s\n", containsIntoHashTable(&hashTable, input) ? "YES" : "NO");
    }

    stamp("Stdin has been processed");

    destruct_HashTable(&hashTable);

    stamp("HashTable has been destroyed");
	return 0;
}
