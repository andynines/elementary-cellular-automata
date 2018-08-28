/*
ecaTerm.c
Copyright (c) 2018 andynines
MIT License
*/

// Includes

#include <stdio.h>
#include <stdlib.h>

#include "eca.h"
#include "ecaout.h"



// Defaults

#define HABITAT_SIZE (80)
#define BUFFER_SIZE (25)
#define BORDER_CODE (WRAP)
#define ALIVE_CELLS (1)
#define SPACING (EVEN)



// Read command line arguments

int getRule(char* str) {
    // Extract a rule int from a character string; return -1 if not possible
    int conversion;

    conversion = atoi(str);

    if (conversion || (str[0] == '0')) { // Make sure 0 wasn't intended
        if ((conversion >= 0) && (conversion <= 255)) {
            return conversion;
        }
    }

    return (-1);
}



// Test

int main(int argc, char* argv[]) {

    int rule;
    Simulation* testSimPtr;

    int getRule(char* str);
    extern Simulation* createSim(int rule,
          	                 int habitatSize,
                                 int genBufferSize,
                                 BoundaryCode borderType,
                                 ConfigCode initCode);
    extern void iterateSim(Simulation* simPtr, int iterations);
    extern void simOut(Simulation* simPtr);
    extern void destroySim(Simulation* simPtr);

    if (argc != 2) { // Take a single argument
        fprintf(stderr, "Usage: %s rule\n", argv[0]);
        return (-1);
    }
    rule = getRule(argv[1]); // Read argument as an int
    if (rule < 0) {
        fprintf(stderr, "Invalid rule\n");
        return (-1);
    }

    testSimPtr = createSim(rule,
                           HABITAT_SIZE,
                           BUFFER_SIZE,
                           BORDER_CODE,
                           (ConfigCode) {.aliveReq = ALIVE_CELLS, .spacing = SPACING});
    iterateSim(testSimPtr, (testSimPtr->genBufferSize) - 1);
    printf("Rule %i\n", rule);
    simOut(testSimPtr);
    destroySim(testSimPtr);

    return 0;
}
