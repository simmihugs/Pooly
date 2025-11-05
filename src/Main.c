#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

#define POOL_SIZE 1024

typedef struct Block {
    struct Block *next;
} Block;

typedef struct MemoryPool {
    Block *freeList;
    unsigned char pool[POOL_SIZE];
} MemoryPool;

void initMemoryPool(MemoryPool *pool) {
    pool->freeList = (Block *)pool->pool;
    Block *current = pool->freeList;

    for (int i=0; i<(POOL_SIZE / sizeof(Block))-1; i++) {
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
    MemoryPool pool;
    initMemoryPool(&pool);
    void *block1 = allocateMemory(&pool);
    void *block2 = allocateMemory(&pool);
    printf("Allocated block1: %p\n", block1);
    printf("Allocated block2: %p\n", block2);

    // Free the blocks
    freeMemory(&pool, block1);
    freeMemory(&pool, block2);
    printf("Freed block1 and block2\n");
    
    return 0;
}
