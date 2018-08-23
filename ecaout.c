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

    currentBlockPtr = &(simPtr->genArr[genIndex].blockArr[0]);
    bitMask = 1 << (blockBits - 1);

    for (cellIndex = 0; cellIndex < (simPtr->habitatSize); ++cellIndex) {
        currentCellState = *currentBlockPtr & bitMask;
        if (currentCellState) {
            cellOut = ALIVE_CELL;
        } else {
            cellOut = DEAD_CELL;
        }
        printf("%c", cellOut);
        bitMask >>= 1;
        if (!bitMask) {
            bitMask = 1 << (blockBits - 1);
            ++currentBlockPtr;
        }
    }

    printf("\n");
}



void simOut(Simulation* simPtr) {
    // Output the entirety of a simulation's generation buffer
    int genIndex;

    void genOut(Simulation *simPtr, int genIndex);

    for (genIndex = 0; genIndex < (simPtr->genBufferSize); ++genIndex) {
        genOut(simPtr, genIndex);
    }
}
