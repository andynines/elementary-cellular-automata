/*
allocs.h
Copyright (c) 2018 andynines
MIT License
*/

#ifndef ALLOCS_H

#define ALLOCS_H



// Includes

#include <stddef.h>



// Function declarations

void* safeMalloc(size_t size);

void* safeRealloc(void* memBlockPtr, size_t size);



#endif
