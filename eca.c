/*
eca.c
Copyright (c) 2018 andynines
MIT License
*/

// Includes

#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#include "eca.h"
#include "utils.h"



// Functions for reading and writing simulation data

CellBlock* getContainerBlock(CellBlock* intlBlockPtr, int cellIndex) {
    // Return pointer to the CellBlock containing the cell at cellIndex
    int blockIndex;
    CellBlock* blockPtr;

    blockIndex = cellIndex / blockBits;
    blockPtr = intlBlockPtr + blockIndex;

    return blockPtr;
}



bool getCellState(CellBlock* intlBlockPtr, int cellIndex) {
    // Determine whether cell at index is alive or dead
    CellBlock* blockPtr;
    int localCellIndex;
    CellBlock bitMask;
    bool cellState;

    CellBlock* getContainerBlock(CellBlock* intlBlockPtr, int cellIndex);

    blockPtr = getContainerBlock(intlBlockPtr, cellIndex);
    localCellIndex = cellIndex % blockBits;
    bitMask = 1 << (blockBits - localCellIndex - 1);
    cellState = *blockPtr & bitMask;

    return cellState;
}



int extractCellNeighborhood(Simulation* simPtr, int genIndex, int cellIndex) {
    // Return integer in [0, 7] representing neighborhood of the cell at cellIndex
    int bitMask;
    int cellNeighborhood;
    int neighborIndex;
    bool wrapOccurred;
    bool currentCellState;

    bool wrap(int* n, int min, int max);
    bool getCellState(CellBlock* intlBlockPtr, int cellIndex);

    --cellIndex; // Begin with left neighbor of desired cell
    bitMask = 4; // Set to 000 ... 000 100
    cellNeighborhood = 0;

    for (neighborIndex = 0; neighborIndex < 3; ++neighborIndex) {
        wrapOccurred = wrap(&cellIndex, 0, (simPtr->habitatSize) - 1);
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



bool determineEvoState(int rule, int cellNeighborhood) {
    // Given a cell's neighborhood, determine its state in the next generation
    int bitMask;
    bool cellState;

    bitMask = 1 << cellNeighborhood;
    cellState = rule & bitMask;

    return cellState;
}



void setCell(CellBlock* intlBlockPtr, int cellIndex, bool state) {
    // Set an individual cell at a certain location to alive or dead
    int blockIndex;
    int localCellIndex;
    CellBlock* targetBlockPtr;
    CellBlock bitMask;

    CellBlock* getContainerBlock(CellBlock* intlBlockPtr, int cellIndex);

    targetBlockPtr = getContainerBlock(intlBlockPtr, cellIndex);
    localCellIndex = cellIndex % blockBits;
    bitMask = 1 << (blockBits - localCellIndex - 1);

    if (state) {
        *targetBlockPtr |= bitMask;
    } else {
        bitMask = ~bitMask;
        *targetBlockPtr &= bitMask;
    }
}



void iterateSim(Simulation* simPtr, int iterations) {
    // Calculate the next state of a simulation and manage the buffer
    int maxGenIndex;
    int iterIndex;
    int genIndex;
    int targetGenIndex;
    int cellIndex;
    int cellNeighborhood;
    bool cellState;

    int extractCellNeighborhood(Simulation* simPtr, int genIndex, int cellIndex);
    bool determineEvoState(int rule, int cellNeighborhood);
    void setCell(CellBlock* intlBlockPtr, int cellIndex, bool state);

    maxGenIndex = (simPtr->genBufferSize) - 1;

    for (iterIndex = 0; iterIndex < iterations; ++iterIndex) {
        if ((simPtr->age) >= maxGenIndex) {
            // If the buffer is full, shift each generation back and overwrite the oldest
            for (genIndex = 0; genIndex < maxGenIndex; ++genIndex) {
                simPtr->genArr[genIndex] = simPtr->genArr[genIndex + 1];
            }
            targetGenIndex = maxGenIndex;
        } else {
            targetGenIndex = (simPtr->age) + 1;
        }

        for (cellIndex = 0; cellIndex < (simPtr->habitatSize); ++cellIndex) {
            cellNeighborhood = extractCellNeighborhood(simPtr, (targetGenIndex - 1), cellIndex);
            cellState = determineEvoState(simPtr->rule, cellNeighborhood);
            setCell(simPtr->genArr[targetGenIndex].blockArr, cellIndex, cellState);
        }

        ++(simPtr->age);
    }
}



// Generation initialization methods

void totalFill(Simulation* simPtr, int genIndex, bool state) {
    // Sets all cells in a generation to alive or dead
    Generation targetGen;
    CellBlock* targetBlockPtr;
    CellBlock bitMask;
    int blockIndex;

    targetGen = simPtr->genArr[genIndex];
    targetBlockPtr = targetGen.blockArr;

    bitMask = 0;
    if (state) {
        bitMask = ~bitMask;
    }

    for (blockIndex = 0; blockIndex < (simPtr->blockReq); ++blockIndex) {
        if (state) {
            *targetBlockPtr |= bitMask;
        } else {
            *targetBlockPtr &= bitMask;
        }
        ++targetBlockPtr;
    }
}



void centeredFill(Simulation* simPtr, int genIndex, bool state) {
    // Center a single cell of the specified state
    int habitatSize;
    int centerCellIndex;
    int cellIndex;
    bool stateBuffer;

    void setCell(CellBlock* intlBlockPtr, int cellIndex, bool state);

    habitatSize = simPtr->habitatSize;
    centerCellIndex = habitatSize / 2;

    for (cellIndex = 0; cellIndex < habitatSize; ++cellIndex) {
        if (cellIndex == centerCellIndex) {
            stateBuffer = state;
        } else {
            stateBuffer = (!state);
        }
        setCell(simPtr->genArr[genIndex].blockArr, cellIndex, stateBuffer);
    }
}



void orderlyFill(Simulation* simPtr, int genIndex, double proportion) {
    // Set a proportion of evenly spaced cells to be alive
    int habitatSize;
    int aliveReq;
    int ratioGcd;
    int clusterSize;
    int aliveClusterSize;
    int cellIndex;
    bool stateBuffer;

    int gcd(int a, int b);
    void setCell(CellBlock* intlBlockPtr, int cellIndex, bool state);

    habitatSize = simPtr->habitatSize;
    aliveReq = habitatSize * proportion;
    ratioGcd = gcd(habitatSize - (habitatSize % 4), aliveReq); // TODO: eliminate filthy hack
    clusterSize = habitatSize / ratioGcd;
    aliveClusterSize = aliveReq / ratioGcd;

    for (cellIndex = 0; cellIndex < habitatSize; ++cellIndex) {
        if ((cellIndex % clusterSize) < aliveClusterSize) {
            stateBuffer = 1;
        } else {
            stateBuffer = 0;
        }
        setCell(simPtr->genArr[genIndex].blockArr, cellIndex, stateBuffer);
    }
}



void probabilisticFill(Simulation* simPtr, int genIndex, double prob) {
    // Randomly set a guarenteed proportion of cells to alive
    int habitatSize;
    int aliveReq;
    int cellIndex;
    double aliveChance;
    bool stateBuffer;

    bool binomProb(double p);
    void setCell(CellBlock* intlBlockPtr, int cellIndex, bool state);

    habitatSize = simPtr->habitatSize;
    aliveReq = habitatSize * prob; // Number of cells we want to be alive

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



void initGen(Simulation* simPtr, int genIndex, ConfigCode initCode) {
    // Initialize a generation with some specified configuration
    int habitatSize;

    void totalFill(Simulation* simPtr, int genIndex, bool state);
    void centeredFill(Simulation* simPtr, int genIndex, bool state);
    void orderlyFill(Simulation* simPtr, int genIndex, double proportion);
    void probabilisticFill(Simulation* simPtr, int genIndex, double prob);

    habitatSize = simPtr->habitatSize;
    // TODO: Rewrite initialization system with non-hardcoded configurations
    switch (initCode) {
        case ALL_DEAD:
            totalFill(simPtr, genIndex, 0);
            break;
        case SINGLE_CENTER_ALIVE:
            centeredFill(simPtr, genIndex, 1);
            break;
        case SINGLE_RANDOM_ALIVE:
            probabilisticFill(simPtr, genIndex, (double) 1 / habitatSize);
            break;
        case QUARTER_SPACED_ALIVE:
            orderlyFill(simPtr, genIndex, 0.25f);
            break;
        case QUARTER_RANDOM_ALIVE:
            probabilisticFill(simPtr, genIndex, 0.25f);
            break;
        case HALF_SPACED_ALIVE:
            orderlyFill(simPtr, genIndex, 0.5f);
            break;
        case HALF_RANDOM_ALIVE:
            probabilisticFill(simPtr, genIndex, 0.5f);
            break;
        case THREEQUARTERS_SPACED_ALIVE:
            orderlyFill(simPtr, genIndex, 0.75f);
            break;
        case THREEQUARTERS_RANDOM_ALIVE:
            probabilisticFill(simPtr, genIndex, 0.75f);
            break;
        case SINGLE_CENTER_DEAD:
            centeredFill(simPtr, genIndex, 0);
            break;
        case SINGLE_RANDOM_DEAD:
            probabilisticFill(simPtr, genIndex, (double) (habitatSize - 1) / habitatSize);
            break;
        case ALL_ALIVE:
            totalFill(simPtr, genIndex, 1);
            break;
    }
}



// Simulation creation and deletion

Simulation* createSim(int rule,
                      int habitatSize,
                      int genBufferSize,
                      BoundaryCode borderType,
                      ConfigCode initCode) {
    // Create a new simulation and initialize its data structures
    Simulation* newSimPtr;
    int blockReq;
    int genIndex;

    void initGen(Simulation* simPtr, int genIndex, ConfigCode initCode);

    srand(time(0));

    newSimPtr = (Simulation*) malloc(sizeof(Simulation));
    newSimPtr->age = 0;
    // Set variables specified by the user
    newSimPtr->rule = rule;
    newSimPtr->habitatSize = habitatSize;
    newSimPtr->genBufferSize = genBufferSize;
    newSimPtr->borderType = borderType;

    // Calculate number of CellBlocks needed to store one generation of data
    blockReq = (blockBits + habitatSize - 1) / blockBits;
    newSimPtr->blockReq = blockReq;

    // Allocate memory to store specified number of generations in buffer
    newSimPtr->genArr = (Generation*) malloc(sizeof(Generation) * genBufferSize);
    for (genIndex = 0; genIndex < genBufferSize; ++genIndex) {
        newSimPtr->genArr[genIndex].blockArr = (CellBlock*) malloc(blockBytes * blockReq);
    }

    initGen(newSimPtr, 0, initCode); // Set up specified initial configuration
    for (genIndex = 1; genIndex < genBufferSize; ++genIndex) {
        // Set all the cells in every other generation of the buffer to dead
        initGen(newSimPtr, genIndex, ALL_DEAD);
    }

    return newSimPtr;
}



void destroySim(Simulation* simPtr) { // TODO: Test this method w/ Valgrind
    // Completely remove a simulation's data from memory
    int genIndex;

    for (genIndex = 0; genIndex < (simPtr->genBufferSize); ++genIndex) {
        free(simPtr->genArr[genIndex].blockArr);
    }
    free(simPtr->genArr);
    free(simPtr);
}
