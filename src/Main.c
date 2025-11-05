#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

#define POOL_SIZE 1024

typedef struct Block {
    struct Block *next;
} Block;

typedef struct MemoryPool {
    Block *freeList;
    unsigned char* pool;
    size_t pool_size;
} MemoryPool;

MemoryPool *createMemoryPool(size_t pool_size) {
    unsigned char* pool = malloc(sizeof(unsigned char) * pool_size);
    if (pool == NULL) {
        return NULL;
    }

    MemoryPool *mpool = malloc(sizeof(MemoryPool));
    mpool->freeList = NULL;
    mpool->pool = pool;
    mpool->pool_size = pool_size;

    return mpool;
}

void freeMemoryPool(MemoryPool *mpool) {
    if (mpool) {
        free(mpool->pool);
        free(mpool);
        printf("Freed memory pool\n");
    }
}


void initMemoryPool(MemoryPool *pool) {
    pool->freeList = (Block *)pool->pool;
    Block *current = pool->freeList;
    size_t block_count = pool->pool_size / sizeof(Block);

    for (int i=0; i<block_count-1; i++) {
        current->next = (Block *)((unsigned char *)current + sizeof(Block));
        current = current->next;
    }
    current->next = NULL;
}

void *allocateMemory(MemoryPool *pool) {
    if (pool->freeList == NULL) {
        printf("Memory pool exhausted\n");
        return NULL;
    }

    Block *block = pool->freeList;
    pool->freeList = block->next;
    return (void *)block;
}

void freeMemory(MemoryPool *pool, void *ptr) {
    Block *block = (Block *)ptr;
    block->next = pool->freeList;
    pool->freeList = block;
}

int main() {
    MemoryPool *pool = createMemoryPool(POOL_SIZE);
    initMemoryPool(pool);
    void *block1 = allocateMemory(pool);
    void *block2 = allocateMemory(pool);
    printf("Allocated block1: %p\n", block1);
    printf("Allocated block2: %p\n", block2);

    // Free the blocks
    freeMemory(pool, block1);
    freeMemory(pool, block2);
    printf("Freed block1 and block2\n");
    freeMemoryPool(pool);
    return 0;
}
