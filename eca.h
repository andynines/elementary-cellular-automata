/*
eca.h
Copyright (c) 2018 andynines
MIT License
*/

#ifndef ECA_H

#define ECA_H



// Includes

#include <stdbool.h>



// Constants

#define blockBytes (sizeof(CellBlock))
#define blockBits (blockBytes * 8)



// Data types

typedef enum {
    // Represent different configurations for generation initializer
    ALL_DEAD,
    SINGLE_CENTER_ALIVE,
    SINGLE_RANDOM_ALIVE,
    QUARTER_SPACED_ALIVE,
    QUARTER_RANDOM_ALIVE,
    HALF_SPACED_ALIVE,
    HALF_RANDOM_ALIVE,
    THREEQUARTERS_SPACED_ALIVE,
    THREEQUARTERS_RANDOM_ALIVE,
    SINGLE_CENTER_DEAD,
    SINGLE_RANDOM_DEAD,
    ALL_ALIVE
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
    Generation* genArr;
} Simulation;



// Function declarations

void iterateSim(Simulation* simPtr, int iterations);

Simulation* createSim(int rule,
                      int habitatSize,
                      int genBufferSize,
                      BoundaryCode borderType,
                      ConfigCode initCode);

void destroySim(Simulation* simPtr);



#endif
