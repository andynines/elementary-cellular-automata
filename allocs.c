/*
allocs.c

Wrappers for standard memory allocation functions that check for allocation 
failures.

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
    /*
    Receive a number of bytes, and attempt to allocate a memory block of that
    size. Return a pointer to the new block of memory so long as the 
    allocation succeeds. Program exits upon failure.
    */
    void* memBlockPtr;

    void* testMemBlock(void* memBlockPtr);

    memBlockPtr = malloc(size);
    return testMemBlock(memBlockPtr);
}



void* safeRealloc(void* memBlockPtr, size_t size) {
    /*
    Receive a pointer to a block of memory and a number of bytes. Attempt to
    resize the memory block to the specified size and return a pointer to the
    resized block of memory if allocation succeeds. Program exits upon failure.
    */
    void* newBlockPtr;

    void* testMemBlock(void* memBlockPtr);

    newBlockPtr = realloc(memBlockPtr, size);
    return testMemBlock(newBlockPtr);
}
