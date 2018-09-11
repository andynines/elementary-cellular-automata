/*
eca.h
Copyright (c) 2018 andynines
MIT License
*/

#ifndef ECA_H

#define ECA_H



// Includes

#include <limits.h>
#include <stdbool.h>



// Constants

#define BLOCK_BYTES (sizeof(CellBlock))
#define BLOCK_BITS (BLOCK_BYTES * CHAR_BIT)



// Data types

typedef enum {
    // Represent the available ways of spacing alive cells
    EVEN,
    RANDOM
} SpacingCode;

typedef struct {
    // Represent different configurations for generation initializer
    int aliveReq;
    SpacingCode spacing;
} ConfigCode;

typedef enum {
    // Specifies how boundaries are treated
    WRAP,
    DEAD_BORDERS,
    ALIVE_BORDERS
} BoundaryCode;

typedef unsigned short int CellBlock;

typedef struct {
    CellBlock* blockArr;
} Generation;

typedef struct {
    int age;
    int rule;
    int habitatSize;
    int genBufferSize;
    int blockReq;
    BoundaryCode borderType;
    ConfigCode initCode;
    Generation* genArr;
} Simulation;



// Function declarations

Simulation* createSim(int rule,
                      int habitatSize,
                      int genBufferSize,
                      BoundaryCode borderType,
                      ConfigCode initCode);

bool getCellState(CellBlock* intlBlockPtr, int cellIndex);

void iterateSim(Simulation* simPtr, int iterations);

void destroySim(Simulation* simPtr);



#endif
