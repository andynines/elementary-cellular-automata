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

    Simulation* simPtr;

    extern Simulation* createUserSim(int argc, char* argv[]);
    extern void iterateSim(Simulation* simPtr, int iterations);
    extern void infoStr(Simulation* simPtr);
    extern void simOut(Simulation* simPtr);
    extern void destroySim(Simulation* simPtr);

    simPtr = createUserSim(argc, argv);
    if (simPtr == (Simulation*) 0) {
        return EXIT_FAILURE;
    } else { 
        iterateSim(simPtr, (simPtr->genBufferSize) - 1); // Just enough to fill buffer
        infoStr(simPtr);
        simOut(simPtr);
        destroySim(simPtr);
        return EXIT_SUCCESS;
    }
}
