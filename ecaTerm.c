/*
ecaTermTest.c
Copyright (c) 2018 andynines
MIT License
*/

// Includes

#include "eca.h"
#include "ecaout.h"



// Constants

#define RULE (30)
#define HABITAT_SIZE (80)
#define BUFFER_SIZE (25)
#define BOUNDARY_CODE (WRAP)
#define INIT_CODE (SINGLE_CENTER_ALIVE)



// Test

int main(void) {

    Simulation* testSimPtr;
    // TODO: Read in variables as cmd line arguments
    testSimPtr = createSim(RULE, HABITAT_SIZE, BUFFER_SIZE, BOUNDARY_CODE, INIT_CODE);
    
    iterateSim(testSimPtr, BUFFER_SIZE - 1);

    simOut(testSimPtr);

    destroySim(testSimPtr);

    return 0;
}
