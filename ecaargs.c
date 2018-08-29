/*
ecaargs.c
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
#include "ecaargs.h"



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

static void usage(char* execName) {
    // Output usage message
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
}



Simulation* createUserSim(int argc, char* argv[]) {
    // If all arguments are valid, return pointer to new simulation
    int rule;
    int habitatSize;
    int genBufferSize;
    BoundaryCode borderType;
    ConfigCode initCode;
    int bufferIndex;

    char* boundaryTypeArgs[] = {"wrap", "dead", "alive"};
    char* spacingArgs[] = {"even", "random"};

    void usage(char* execName);
    bool stageIntArg(char* arg, int minVal, int maxVal, int* writeToPtr);
    bool stageStrArg(char* arg, int matches, char* mustMatch[matches], int* writeToPtr);
    extern Simulation* createSim(int rule,
                                 int habitatSize,
                                 int genBufferSize,
                                 BoundaryCode borderType,
                                 ConfigCode initCode);

    if (argc != TOTAL_ARGC) {
        usage(argv[0]);
        return (Simulation*) 0;
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
            printf("%i\n", argBuffer[bufferIndex]);
            usage(argv[0]);
            return (Simulation*) 0;
        }
    }

    return createSim(rule, habitatSize, genBufferSize, borderType, initCode);
}
