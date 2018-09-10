/*
allocs.c
Copyright (c) 2018 andynines
MIT License
*/

// Includes

#include <stdio.h>
#include <stdlib.h>



// Memory block tester

static void* testMemBlock(void* memBlockPtr) {
    // If pointer is null, quit, else return the pointer
    if (memBlockPtr == NULL) {
        fprintf(stderr, "Error: Not enough memory\n");
        exit(EXIT_FAILURE);
    } else {
        return memBlockPtr;
    }
}



// Memory allocation function wrappers

void* safeMalloc(size_t size) {
    // Allocate requested amount of memory if space is available, and return pointer to block
    void* memBlockPtr;

    void* testMemBlock(void* memBlockPtr);

    memBlockPtr = malloc(size);
    return testMemBlock(memBlockPtr);
}



void* safeRealloc(void* memBlockPtr, size_t size) {
    // Reallocate a block of memory to the requested size, or return null pointe if impossible
    void* newBlockPtr;

    void* testMemBlock(void* memBlockPtr);

    newBlockPtr = realloc(memBlockPtr, size);
    return testMemBlock(newBlockPtr);
}
