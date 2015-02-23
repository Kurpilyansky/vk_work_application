#include <stdlib.h>
#include <memory.h>
#include <assert.h>
#include <stdbool.h>

const int HASH_TABLE_FACTOR = 14;
const int EXPAND_FACTOR = 2;

typedef unsigned long long HashType;
const HashType UNDEF_HASH = (HashType)-1LL;

struct HashTable {
    size_t hashSize;
    size_t size;
    HashType* hashes;
};

void init_HashTable(struct HashTable* hashTablePtr, size_t hashSize) {
    assert(hashSize > 0);
    while (hashSize & (hashSize - 1)) {
        hashSize += hashSize - (hashSize & (hashSize - 1));
    }
    hashTablePtr->hashSize = hashSize;
    hashTablePtr->size = 0;

    hashTablePtr->hashes = malloc(hashSize * HASH_TABLE_FACTOR * sizeof(HashType));
    assert(hashTablePtr->hashes != NULL);
    memset(hashTablePtr->hashes, 0xff, hashSize * HASH_TABLE_FACTOR * sizeof(size_t));
}

void init_HashTableByCapacity(struct HashTable* hashTablePtr, size_t capacity) {
    init_HashTable(hashTablePtr, capacity * 4 / HASH_TABLE_FACTOR + 1);
}

void destruct_HashTable(struct HashTable* hashTablePtr) {
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
bool tryAddIntoHashTable_Internal(struct HashTable* hashTablePtr, HashType hash);

void rehashHashTable(struct HashTable* hashTablePtr) {
    struct HashTable newHashTable;
    init_HashTable(&newHashTable, hashTablePtr->hashSize * EXPAND_FACTOR);

    size_t i, j;
    const HashType* row;
    for (i = 0, row = hashTablePtr->hashes; i < hashTablePtr->hashSize; ++i, row += HASH_TABLE_FACTOR) {
        for (j = 0; row[j] != UNDEF_HASH; ++j) {
            tryAddIntoHashTable_Internal(&newHashTable, row[j]);
        }
    }

    destruct_HashTable(hashTablePtr);
    memcpy((char*)hashTablePtr, (char*)&newHashTable, sizeof(struct HashTable));
}

bool tryAddIntoHashTable(struct HashTable* hashTablePtr, const char* s) {
    tryAddIntoHashTable_Internal(hashTablePtr, calculateHash(s));
}

bool tryAddIntoHashTable_Internal(struct HashTable* hashTablePtr, HashType hash) {
    while (true) {
        size_t position = hash % hashTablePtr->hashSize;
        HashType* row = hashTablePtr->hashes + position * HASH_TABLE_FACTOR;
        size_t next = 0;
        while (row[next] != hash && row[next] != UNDEF_HASH) {
            ++next;
        }
        if (row[next] == hash) {
            return false;
        }
        if (next + 1 < HASH_TABLE_FACTOR) {
            hashTablePtr->size++;
            row[next] = hash;
            return true;
        }
        rehashHashTable(hashTablePtr);
    }
}

bool containsIntoHashTable(struct HashTable* hashTablePtr, const char * s) {
    long long hash = calculateHash(s);
    size_t position = hash % hashTablePtr->hashSize;
    HashType* row = hashTablePtr->hashes + position * HASH_TABLE_FACTOR;
    size_t next = 0;
    while (row[next] != hash && row[next] != UNDEF_HASH) {
        ++next;
    }
    return row[next] == hash;
}

