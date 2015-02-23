#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>

#include "hash_table.h"

bool readLine(FILE* file, char * s, int maxLength) {
    static char buffer[64 * 1024];
    size_t length = 0;
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        size_t buffer_length = strlen(buffer);
        assert(buffer_length > 0);
        assert(length + buffer_length <= maxLength);
        memcpy(s + length, buffer, (buffer_length + 1) * sizeof(char));
        length += buffer_length;
        if (s[length - 1] == '\n') {
            s[--length] = '\0';
            return true;
        }
    }
    return length > 0;
}

#define MAX_SIZE (128 * 1024 * 1024)
char text[MAX_SIZE + 1];

void indexLinesInFile(const char * filename, struct HashTable* hashTablePtr) {
    FILE* file = fopen(filename, "r");
    assert(file != NULL);

    size_t offset = 0;
    char* s;
    while (readLine(file, s = text + offset, sizeof(text) - 1 - offset)) {
        size_t length = strlen(s);
        if (tryAddIntoHashTable(hashTablePtr, s)) {
            offset += length + 1;
        }
    }
}

int main(int argc, char** argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    struct HashTable hashTable;
    init_HashTable(&hashTable, 10);

    indexLinesInFile(argv[1], &hashTable);

    static char input[MAX_SIZE + 1];
    while (readLine(stdin, input, sizeof(input) - 1)) {
        if (strcmp(input, "exit") == 0) {
            break;
        }
        printf("%s\n", containsIntoHashTable(&hashTable, input) ? "YES" : "NO");
    }

    destruct_HashTable(&hashTable);
	return 0;
}
