#include <stdlib.h>
#include <memory.h>
#include <assert.h>
#include <stdbool.h>

const int HASH_TABLE_FACTOR = 100;

struct HashTable {
    size_t hashSize;
    size_t size;
    size_t* first;
    size_t* next;
    const char** values;
};

void init_HashTable(struct HashTable* hashTablePtr, size_t hashSize) {
    assert(hashSize > 0);
    hashTablePtr->hashSize = hashSize;
    hashTablePtr->size = 0;
    hashTablePtr->first = malloc(hashSize * sizeof(size_t));
    hashTablePtr->next = malloc(hashSize * HASH_TABLE_FACTOR * sizeof(size_t));
    hashTablePtr->values = malloc(hashSize * HASH_TABLE_FACTOR * sizeof(char*));
    memset(hashTablePtr->first, 0xff, hashSize * sizeof(size_t));
}

void destruct_HashTable(struct HashTable* hashTablePtr) {
    if (hashTablePtr->first != NULL) {
        free(hashTablePtr->first);
        hashTablePtr->first = NULL;
    }
    if (hashTablePtr->next != NULL) {
        free(hashTablePtr->next);
        hashTablePtr->next = NULL;
    }
    if (hashTablePtr->values != NULL) {
        free(hashTablePtr->values);
        hashTablePtr->values = NULL;
    }
}

long long calculateHash(const char * s) {
    static const long long MOD = (long long)(1e9) + 7;
    static const long long P = 997;
    long long hash = 0;
    size_t i;
    for (i = 0; s[i] != '\0'; ++i) {
        hash = (hash * P + s[i]) % MOD;
    }
    return hash;
}

void rehashHashTable(struct HashTable* hashTablePtr);
bool tryAddIntoHashTable(struct HashTable* hashTablePtr, const char* s);
bool containsIntoHashTable(struct HashTable* hashTablePtr, const char* s);

void rehashHashTable(struct HashTable* hashTablePtr) {
    struct HashTable newHashTable;
    init_HashTable(&newHashTable, hashTablePtr->hashSize * 2);
    size_t i;
    for (i = 0; i < hashTablePtr->size; ++i) {
        tryAddIntoHashTable(&newHashTable, hashTablePtr->values[i]);
    }
    destruct_HashTable(hashTablePtr);
    memcpy((char*)hashTablePtr, (char*)&newHashTable, sizeof(struct HashTable));
}

bool tryAddIntoHashTable(struct HashTable* hashTablePtr, const char* s) {
    long long hash = calculateHash(s);
    while (true) {
        size_t position = hash % hashTablePtr->hashSize;
        size_t current = hashTablePtr->first[position];
        size_t count = 0;
        while (current != -1 && strcmp(hashTablePtr->values[current], s) != 0) {
            current = hashTablePtr->next[current];
            ++count;
        }
        if (current != -1) {
            return false;
        }
        if (count < HASH_TABLE_FACTOR) {
            size_t index = hashTablePtr->size++;
            hashTablePtr->values[index] = s;
            hashTablePtr->next[index] = hashTablePtr->first[position];
            hashTablePtr->first[position] = index;
            return true;
        }
        rehashHashTable(hashTablePtr);
    }
}

bool containsIntoHashTable(struct HashTable* hashTablePtr, const char * s) {
    long long hash = calculateHash(s);
    size_t position = hash % hashTablePtr->hashSize;
    size_t current = hashTablePtr->first[position];
    while (current != -1 && strcmp(hashTablePtr->values[current], s) != 0) {
        current = hashTablePtr->next[current];
    }
    return current != -1;
}

