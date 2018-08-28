/*
utils.c
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
    // Returns one-trial experimental outcome of a binomial probability
    double zTest;

    zTest = (double) rand() / RAND_MAX;

    return (zTest <= p);
}



int min(int a, int b) {
    // Find the smaller of two numbers
    if (a < b) {
        return a;
    } else {
        return b;
    }
}



bool wrap(int* n, int nMax) {
    // Keep a number between 0 and a specified max; return true if wrap occurs
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
