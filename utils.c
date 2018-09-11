/*
utils.c

Miscellaneous mathematical methods used in the elementary cellular automata 
simulation engine.

Copyright (c) 2018 andynines
MIT License
*/

// Includes

#include <stdbool.h>
#include <stdlib.h>

#include "utils.h"



// Utility methods

static int absVal(int n) {
    // Return the absolute value of an integer argument
    if (n < 0) {
        n *= (-1);
    }
    return n;
}



bool binomProb(double p) {
    /*
    Given some double representing the probability of success of a two-outcome
    event, return a boolean representing the success of this event during a
    single trial. True means success; false means failure.
    */
    double zTest;

    zTest = (double) rand() / RAND_MAX;

    return (zTest <= p);
}



int min(int a, int b) {
    /*
    Given two integer arguments, return back the smaller of those two values.
    */
    if (a < b) {
        return a;
    } else {
        return b;
    }
}



bool wrap(int* n, int nMax) {
    /*
    Given a pointer to an integer and a maximum value for this number, test to
    see if this number is within the inclusive range between 0 and the specified
    maximum. If it is outside this range, wrap the number back into it using
    modulo and return true. Otherwise, return false.
    */
    int range;

    int absVal(int n);

    range = nMax + 1;

    if (*n > nMax) {
        *n %= range;
        return true;
    } else if (*n < 0) {
        *n = range - (absVal(*n) % range);
        return true;
    } else {
        return false;
    }
}
