/*
eca.c

Space-efficient elementary cellular automaton simulation library.

Copyright (c) 2018 andynines
MIT License
*/

// Includes

#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#include "eca.h"
#include "allocs.h"
#include "fracs.h"
#include "utils.h"



// Functions for reading and writing simulation data

static void copyGen(CellBlock* sourceIntlPtr, CellBlock* targetIntlPtr, int blockReq) {
    // Copy one generation's cells to another
    int blockIndex;

    for (blockIndex = 0; blockIndex < blockReq; ++blockIndex) {
        targetIntlPtr[blockIndex] = sourceIntlPtr[blockIndex];
    }
}



static bool determineEvoState(int rule, int cellNeighborhood) {
    // Given a cell's neighborhood, determine its state in the next generation
    int bitMask;
    bool cellState;

    bitMask = 1 << cellNeighborhood;
    cellState = rule & bitMask;

    return cellState;
}



static CellBlock* getContainerBlock(CellBlock* intlBlockPtr, int cellIndex) {
    // Return pointer to the CellBlock containing the cell at cellIndex
    int blockIndex;
    CellBlock* blockPtr;

    blockIndex = cellIndex / BLOCK_BITS;
    blockPtr = intlBlockPtr + blockIndex;

    return blockPtr;
}



static void setCell(CellBlock* intlBlockPtr, int cellIndex, bool state) {
    // Set an individual cell at a certain location to alive or dead
    int localCellIndex;
    CellBlock* targetBlockPtr;
    CellBlock bitMask;

    CellBlock* getContainerBlock(CellBlock* intlBlockPtr, int cellIndex);

    targetBlockPtr = getContainerBlock(intlBlockPtr, cellIndex);
    localCellIndex = cellIndex % BLOCK_BITS;
    bitMask = 1 << (BLOCK_BITS - localCellIndex - 1);

    if (state) {
        *targetBlockPtr |= bitMask;
    } else {
        bitMask = ~bitMask;
        *targetBlockPtr &= bitMask;
    }
}



static int extractCellNeighborhood(Simulation* simPtr, int genIndex, int cellIndex) {
    // Return integer in [0,7] representing neighborhood of the cell at cellIndex
    int bitMask;
    int cellNeighborhood;
    int neighborIndex;
    bool wrapOccurred;
    bool currentCellState;

    extern bool wrap(int* n, int nMax);
    bool getCellState(CellBlock* intlBlockPtr, int cellIndex);

    --cellIndex; // Begin with left neighbor of desired cell
    bitMask = 4; // Set to 000 ... 000 100
    cellNeighborhood = 0;

    for (neighborIndex = 0; neighborIndex < 3; ++neighborIndex) {
        wrapOccurred = wrap(&cellIndex, (simPtr->habitatSize) - 1);
        if (wrapOccurred) {
            switch (simPtr->borderType) {
                case WRAP:
                    currentCellState = getCellState(simPtr->genArr[genIndex].blockArr, cellIndex);
                    break;
                case DEAD_BORDERS:
                    currentCellState = false;
                    break;
                case ALIVE_BORDERS:
                    currentCellState = true;
                    break;
            }
        } else {
            currentCellState = getCellState(simPtr->genArr[genIndex].blockArr, cellIndex);
        }
        if (currentCellState) {
            cellNeighborhood |= bitMask;
        }
        ++cellIndex;
        bitMask >>= 1;
    }

    return cellNeighborhood;
}



static void rotateCells(Simulation* simPtr, int genIndex, int rotVector) {
    // Rotate cells in the specified direction; positive vectors move right
    Generation tempGen;
    int cellIndex;
    int tempIndex;
    bool cellState;

    void copyGen(CellBlock* sourceIntlPtr, CellBlock* targetIntlPtr, int blockReq);
    extern bool wrap(int* n, int nMax);
    bool getCellState(CellBlock* intlBlockPtr, int cellIndex);
    void setCell(CellBlock* intlBlockPtr, int cellIndex, bool state);

    tempGen.blockArr = (CellBlock*) safeMalloc(BLOCK_BYTES * (simPtr->blockReq));
    copyGen(simPtr->genArr[genIndex].blockArr, tempGen.blockArr, simPtr->blockReq);
    
    for (cellIndex = 0; cellIndex < (simPtr->habitatSize); ++cellIndex) {
        tempIndex = cellIndex + (simPtr->habitatSize) - rotVector;
        wrap(&tempIndex, (simPtr->habitatSize) - 1);
        cellState = getCellState(tempGen.blockArr, tempIndex);
        setCell(simPtr->genArr[genIndex].blockArr, cellIndex, cellState);
    }

    free(tempGen.blockArr);
}



// Generation initialization methods

static void orderlyFill(Simulation* simPtr, int genIndex, int aliveReq) {
    // Set some number of evenly spaced cells to be alive
    int habitatSize;
    Fraction* cellRatioPtr;
    int num;
    int denom;
    int cellIndex;
    bool stateBuffer;

    extern Fraction* createFrac(int num, int denom);
    extern void simplfyFrac(Fraction* fracPtr);
    extern void destroyFrac(Fraction* fracPtr);
    void setCell(CellBlock* intlBlockPtr, int cellIndex, bool state);
    void rotateCells(Simulation* simPtr, int genIndex, int rotVector);

    habitatSize = simPtr->habitatSize;
    cellRatioPtr = createFrac(aliveReq, habitatSize);
    simplifyFrac(cellRatioPtr);
    num = cellRatioPtr->num;
    denom = cellRatioPtr->denom;

    for (cellIndex = 0; cellIndex < habitatSize; ++cellIndex) {
        if (cellIndex % denom < num) {
            stateBuffer = 1;
        } else {
            stateBuffer = 0;
        }
        setCell(simPtr->genArr[genIndex].blockArr, cellIndex, stateBuffer);
    }
    // Center the configuration as much as possible
    rotateCells(simPtr, genIndex, (denom - num) / 2);

    destroyFrac(cellRatioPtr);
}



static void probabilisticFill(Simulation* simPtr, int genIndex, int aliveReq) {
    // Randomly set a guaranteed number of cells to alive
    int habitatSize;
    int cellIndex;
    double aliveChance;
    bool stateBuffer;

    extern bool binomProb(double p);
    void setCell(CellBlock* intlBlockPtr, int cellIndex, bool state);

    habitatSize = simPtr->habitatSize;

    for (cellIndex = 0; cellIndex < habitatSize; ++cellIndex) {
        // Adjust probability based on available space and how many cells need to be alive
        aliveChance = (double) aliveReq / (habitatSize - cellIndex);
        if (binomProb(aliveChance)) {
            stateBuffer = 1;
            --aliveReq;
        } else {
            stateBuffer = 0;
        }
        setCell(simPtr->genArr[genIndex].blockArr, cellIndex, stateBuffer);
    }
}



static void initGen(Simulation* simPtr, int genIndex, ConfigCode initCode) {
    // Initialize a generation with some specified configuration
    void orderlyFill(Simulation* simPtr, int genIndex, int aliveReq);
    void probabilisticFill(Simulation* simPtr, int genIndex, int aliveReq);

    switch (initCode.spacing) {
        case EVEN:
            orderlyFill(simPtr, genIndex, initCode.aliveReq);
            break;
        case RANDOM:
            probabilisticFill(simPtr, genIndex, initCode.aliveReq);
            break;
    }
}



// Functions for operating simulations

Simulation* createSim(int rule,
                      int habitatSize,
                      int genBufferSize,
                      BoundaryCode borderType,
                      ConfigCode initCode) {
    /*
    Given a rule from Wolfram's numbering scheme, a number of cells belonging to
    a single generation, a number of generations to store on memory, a method of
    interpreting the neighborhoods of edge cells, and a code specifying how the
    initial generation should be populated, build a simulation according to 
    these attributes and return a pointer to it.
    */
    Simulation* newSimPtr;
    int blockReq;
    int genIndex;

    void initGen(Simulation* simPtr, int genIndex, ConfigCode initCode);

    srand(time(0));

    newSimPtr = (Simulation*) safeMalloc(sizeof(Simulation));
    newSimPtr->age = 0;
    // Set variables specified by the user
    newSimPtr->rule = rule;
    newSimPtr->habitatSize = habitatSize;
    newSimPtr->genBufferSize = genBufferSize;
    newSimPtr->borderType = borderType;
    newSimPtr->initCode = initCode;

    // Calculate number of CellBlocks needed to store one generation of data
    blockReq = (BLOCK_BITS + habitatSize - 1) / BLOCK_BITS; // Ceiling division
    newSimPtr->blockReq = blockReq;

    // Allocate memory to store specified number of generations in buffer
    newSimPtr->genArr = (Generation*) safeMalloc(sizeof(Generation) * genBufferSize);
    for (genIndex = 0; genIndex < genBufferSize; ++genIndex) {
        newSimPtr->genArr[genIndex].blockArr = (CellBlock*) safeMalloc(BLOCK_BYTES * blockReq);
    }

    initGen(newSimPtr, 0, initCode); // Set up specified initial configuration
    for (genIndex = 1; genIndex < genBufferSize; ++genIndex) {
        // Set all the cells in every other generation of the buffer to dead
        initGen(newSimPtr, genIndex, (ConfigCode) {.aliveReq = 0, .spacing = EVEN});
    }

    return newSimPtr;
}



bool getCellState(CellBlock* intlBlockPtr, int cellIndex) {
    /*
    Locate a particular cell bit based off of a pointer to its generation's
    memory block and its index relative to that entire generation. Return a
    boolean representing the cell's state; live cells return true and dead cells
    return false.
    */
    CellBlock* blockPtr;
    int localCellIndex;
    CellBlock bitMask;
    bool cellState;

    CellBlock* getContainerBlock(CellBlock* intlBlockPtr, int cellIndex);

    blockPtr = getContainerBlock(intlBlockPtr, cellIndex);
    localCellIndex = cellIndex % BLOCK_BITS;
    bitMask = 1 << (BLOCK_BITS - localCellIndex - 1);
    cellState = *blockPtr & bitMask;

    return cellState;
}



void iterateSim(Simulation* simPtr, int iterations) {
    /*
    Knowing the latest states of each cell in a simulation, calculate their next
    state. Place the resulting new generation in the buffer, deleting the oldest
    generation in the buffer if necassary in order to make room.
    */
    int maxGenIndex;
    int targetGenIndex;
    int genIndex;
    int cellIndex;
    int cellNeighborhood;
    bool cellState;

    extern int min(int a, int b);
    void copyGen(CellBlock* sourceIntlPtr, CellBlock* targetIntlPtr, int blockReq);
    int extractCellNeighborhood(Simulation* simPtr, int genIndex, int cellIndex);
    bool determineEvoState(int rule, int cellNeighborhood);
    void setCell(CellBlock* intlBlockPtr, int cellIndex, bool state);

    maxGenIndex = (simPtr->genBufferSize) - 1;

    for (; iterations > 0; --iterations) {
        targetGenIndex = min((simPtr->age) + 1, maxGenIndex);
        // If buffer is full, shift every gen back and overwrite the oldest
        if ((simPtr->age) >= maxGenIndex) {
            for (genIndex = 0; genIndex < maxGenIndex; ++genIndex) {
                copyGen(simPtr->genArr[genIndex + 1].blockArr,
                        simPtr->genArr[genIndex].blockArr,
                        simPtr->blockReq);
            }
        }
        for (cellIndex = 0; cellIndex < (simPtr->habitatSize); ++cellIndex) {
            cellNeighborhood = extractCellNeighborhood(simPtr, (targetGenIndex - 1), cellIndex);
            cellState = determineEvoState(simPtr->rule, cellNeighborhood);
            setCell(simPtr->genArr[targetGenIndex].blockArr, cellIndex, cellState);
        }
        ++(simPtr->age);
    }
}



void destroySim(Simulation* simPtr) {
    /*
    Remove a simulation's data from memory using its pointer.
    */
    int genIndex;

    for (genIndex = 0; genIndex < (simPtr->genBufferSize); ++genIndex) {
        free(simPtr->genArr[genIndex].blockArr);
    }
    free(simPtr->genArr);
    free(simPtr);
}
