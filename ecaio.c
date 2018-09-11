/*
ecaio.h

Methods for reading in arguments that specify different attributes of a 
simulation, and outputting text representations of simulations in the terminal.

Copyright (c) 2018 andynines
MIT License
*/

// Includes

#include <ctype.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "eca.h"
#include "ecaio.h"



// Argument filtering methods

static bool stageIntArg(char* arg, int minVal, int maxVal, int* writeToPtr) {
    // Read an integer arugment and write it to the specified location if in range
    int conversion;

    conversion = atoi(arg);
    
    if (conversion || (arg[0] == '0')) { // Make sure 0 wasn't intended
        if ((minVal <= conversion) && (conversion <= maxVal)) { // Enforce range
            *writeToPtr = conversion;
            return true;
        }
    }

    return false;
}



static bool stageStrArg(char* arg, int matches, char* mustMatch[matches], int* writeToPtr) {
    // Read in string argument and write index of matching string to specified location
    int charIndex;
    bool matchFound;
    int matchIndex;
    // Convert to lowercase
    for (charIndex = 0; arg[charIndex] != '\0'; ++charIndex) {
        arg[charIndex] = tolower(arg[charIndex]);
    }

    matchFound = false;
    for (matchIndex = 0; matchIndex < matches; ++matchIndex) {
        if (strcmp(arg, mustMatch[matchIndex]) == 0) {
            matchFound = true;
            break;
        }
    }

    if (matchFound) {
        *writeToPtr = matchIndex;
        return true;
    } else {
        return false;
    }
}



// Argument processing routine

static Simulation* usage(char* execName) {
    // Output usage message and return a null sim pointer
    fprintf(stderr,
            "Usage: %s rule habitat-width buffer-height boundary-type cells-alive spacing\n"
            "\n"
            "rule:              integer from 0 to 255\n"
            "habitat-width:     integer greater than 0\n"
            "buffer-height:     integer greater than 1\n"
            "boundary-type:     wrap, dead, alive\n"
            "cells-alive:       integer from 0 to habitat-width\n"
            "spacing:           even, random\n",
            execName);

    return (Simulation*) 0;
}



Simulation* createUserSim(int argc, char* argv[]) {
    /*
    This method wraps the createSim method in eca.c, reading in an argument 
    vector and extracting its information in order to create a simulation 
    specified by a user on the command line. If all arguments are acceptable,
    it passes back the pointer received from the sim creator. Otherwise, it
    displays a usage message.
    */
    int rule;
    int habitatSize;
    int genBufferSize;
    BoundaryCode borderType;
    ConfigCode initCode;
    int bufferIndex;

    char* boundaryTypeArgs[] = {"wrap", "dead", "alive"};
    char* spacingArgs[] = {"even", "random"};

    Simulation* usage(char* execName);
    bool stageIntArg(char* arg, int minVal, int maxVal, int* writeToPtr);
    bool stageStrArg(char* arg, int matches, char* mustMatch[matches], int* writeToPtr);
    extern Simulation* createSim(int rule,
                                 int habitatSize,
                                 int genBufferSize,
                                 BoundaryCode borderType,
                                 ConfigCode initCode);

    if (argc != TOTAL_ARGC) {
        return usage(argv[0]);
    }

    bool argBuffer[ECA_ARGC] = {
        stageIntArg(argv[1], 0, 255, &rule),
        stageIntArg(argv[2], 1, INT_MAX, &habitatSize),
        stageIntArg(argv[3], 2, INT_MAX, &genBufferSize),
        stageStrArg(argv[4], 3, boundaryTypeArgs, (int*) &borderType),
        stageIntArg(argv[5], 0, habitatSize, &(initCode.aliveReq)),
        stageStrArg(argv[6], 2, spacingArgs, (int*) &(initCode.spacing))};
    
    for (bufferIndex = 0; bufferIndex < ECA_ARGC; ++bufferIndex) {
        if (!argBuffer[bufferIndex]) {
            return usage(argv[0]);
        }
    }

    return createSim(rule, habitatSize, genBufferSize, borderType, initCode);
}



// Printing algorithms

void infoStr(Simulation* simPtr) {
    /*
    Uses the information from a simulation pointer to print a formatted string
    containing relevant information about that simulation.
    */
    char* const borders[] = {"Wrapped", "Dead", "Live"};
    char* const spacings[] = {"evenly", "randomly"};

    printf("Rule %i | %ix%i | %s borders | %i initial live cell%sspaced %s\n",
           simPtr->rule,
           simPtr->habitatSize,
           simPtr->genBufferSize,
           borders[simPtr->borderType],
           simPtr->initCode.aliveReq,
           (simPtr->initCode.aliveReq == 1)? " " : "s ",
           spacings[simPtr->initCode.spacing]);
}



void genOut(Simulation* simPtr, int genIndex) {
    /*
    Reads the data contained in a simulation at the specified index in the 
    generation buffer. Prints a line of text, each character of which represents
    a live or dead cell according to the OutChar enum in ecaio.h.
    */
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
    /*
    Wraps the genOut method in order to print every generation in the buffer of 
    a given simulation, printing the oldest buffer line first.
    */
    int genIndex;

    void genOut(Simulation *simPtr, int genIndex);
   
    for (genIndex = 0; genIndex < (simPtr->genBufferSize); ++genIndex) {
        genOut(simPtr, genIndex);
    }
}
