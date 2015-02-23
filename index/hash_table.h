#include <stdlib.h>
#include <memory.h>
#include <assert.h>
#include <stdbool.h>

const int HASH_TABLE_FACTOR = 10;
const int EXPAND_FACTOR = 2;

typedef unsigned long long HashType;

struct HashTable {
    size_t hashSize;
    size_t size;
    size_t* first;
    size_t* next;
    const char** values;
    HashType* hashes;
};

void init_HashTable(struct HashTable* hashTablePtr, size_t hashSize) {
    assert(hashSize > 0);
    while (hashSize & (hashSize - 1)) {
        hashSize += hashSize - (hashSize & (hashSize - 1));
    }
    hashTablePtr->hashSize = hashSize;
    hashTablePtr->size = 0;

    hashTablePtr->first = malloc(hashSize * sizeof(size_t));
    assert(hashTablePtr->first != NULL);
    memset(hashTablePtr->first, 0xff, hashSize * sizeof(size_t));

    hashTablePtr->next = malloc(hashSize * HASH_TABLE_FACTOR * sizeof(size_t));
    assert(hashTablePtr->next != NULL);

    hashTablePtr->values = malloc(hashSize * HASH_TABLE_FACTOR * sizeof(char*));
    assert(hashTablePtr->values != NULL);

    hashTablePtr->hashes = malloc(hashSize * HASH_TABLE_FACTOR * sizeof(HashType));
    assert(hashTablePtr->hashes != NULL);
}

void init_HashTableByCapacity(struct HashTable* hashTablePtr, size_t capacity) {
    init_HashTable(hashTablePtr, capacity * 4 / HASH_TABLE_FACTOR + 1);
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
    if (hashTablePtr->hashes != NULL) {
        free(hashTablePtr->hashes);
        hashTablePtr->hashes = NULL;
    }
}

HashType calculateHash(const char * s) {
//    static const HashType MOD = (HashType)(1e9) + 7;
    static const HashType P = 997;
    HashType hash = 0;
    size_t i;
    for (i = 0; s[i] != '\0'; ++i) {
        hash = hash * P + s[i];
//        hash %= MOD;
    }
    return hash;
}

void rehashHashTable(struct HashTable* hashTablePtr);
bool tryAddIntoHashTable(struct HashTable* hashTablePtr, const char* s);
bool containsIntoHashTable(struct HashTable* hashTablePtr, const char* s);
bool tryAddIntoHashTable_Internal(struct HashTable* hashTablePtr, const char* s, HashType hash, bool sureNew);

void rehashHashTable(struct HashTable* hashTablePtr) {
    struct HashTable newHashTable;
    init_HashTable(&newHashTable, hashTablePtr->hashSize * EXPAND_FACTOR);
    size_t i;
    for (i = 0; i < hashTablePtr->size; ++i) {
        tryAddIntoHashTable_Internal(&newHashTable, hashTablePtr->values[i], hashTablePtr->hashes[i], true);
    }
    destruct_HashTable(hashTablePtr);
    memcpy((char*)hashTablePtr, (char*)&newHashTable, sizeof(struct HashTable));
}

bool tryAddIntoHashTable(struct HashTable* hashTablePtr, const char* s) {
    tryAddIntoHashTable_Internal(hashTablePtr, s, calculateHash(s), false);
}

bool tryAddIntoHashTable_Internal(struct HashTable* hashTablePtr, const char* s, HashType hash, bool sureNew) {
    while (true) {
        size_t position = hash % hashTablePtr->hashSize;
        size_t current = sureNew ? -1 : hashTablePtr->first[position];
        size_t count = 0;
        while (current != -1 && (hashTablePtr->hashes[current] != hash || strcmp(hashTablePtr->values[current], s) != 0)) {
            current = hashTablePtr->next[current];
            ++count;
        }
        if (current != -1) {
            return false;
        }
        if (count < HASH_TABLE_FACTOR) {
            size_t index = hashTablePtr->size++;
            hashTablePtr->values[index] = s;
            hashTablePtr->hashes[index] = hash;
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

