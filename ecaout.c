/*
ecaout.c
Copyright (c) 2018 andynines
MIT License
*/

// Includes

#include <stdbool.h>
#include <stdio.h>

#include "eca.h"
#include "ecaout.h"



// Printing algorithms

void genOut(Simulation* simPtr, int genIndex) {
    // Print text representation of a single generation to stdout
    CellBlock* currentBlockPtr;
    CellBlock bitMask;
    int cellIndex;
    bool currentCellState;
    OutChar cellOut;

    currentBlockPtr = simPtr->genArr[genIndex].blockArr;
    bitMask = 1 << (BLOCK_BITS - 1); // Set to 100 000 ... 000

    for (cellIndex = 0; cellIndex < (simPtr->habitatSize); ++cellIndex) {
        currentCellState = *currentBlockPtr & bitMask;
        if (currentCellState) {
            cellOut = ALIVE_CELL;
        } else {
            cellOut = DEAD_CELL;
        }
        putchar(cellOut);
        bitMask >>= 1;
        if (!bitMask) {
            bitMask = 1 << (BLOCK_BITS - 1);
            ++currentBlockPtr;
        }
    }

    putchar('\n');
}



void simOut(Simulation* simPtr) {
    // Output the entirety of a simulation's generation buffer
    int genIndex;

    char* const borders[] = {"Wrapped", "Dead borders", "Live borders"};
    char* const spacings[] = {"evenly", "randomly"};

    void genOut(Simulation *simPtr, int genIndex);

    printf("Rule %i | %ix%i | %s | %i initial live cells spaced %s\n",
        simPtr->rule,
        simPtr->habitatSize,
        simPtr->genBufferSize,
        borders[simPtr->borderType],
        simPtr->initCode.aliveReq,
        spacings[simPtr->initCode.spacing]);

    for (genIndex = 0; genIndex < (simPtr->genBufferSize); ++genIndex) {
        genOut(simPtr, genIndex);
    }
}
