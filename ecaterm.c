/*
ecaterm.c
Copyright (c) 2018 andynines
MIT License
*/

// Includes

#include <stdlib.h>

#include "eca.h"
#include "ecaio.h"



// Test

int main(int argc, char* argv[]) {

    Simulation* testSimPtr;

    extern Simulation* createUserSim(int argc, char* argv[]);
    extern void iterateSim(Simulation* simPtr, int iterations);
    extern void simOut(Simulation* simPtr);
    extern void destroySim(Simulation* simPtr);

    testSimPtr = createUserSim(argc, argv);
    if (testSimPtr == (Simulation*) 0) {
        return EXIT_FAILURE;
    } else { 
        iterateSim(testSimPtr, (testSimPtr->genBufferSize) - 1);
        simOut(testSimPtr);
        destroySim(testSimPtr);
        return EXIT_SUCCESS;
    }
}
