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

bool binomProb(double p) {
    // Returns one-trial experimental outcome of a binomial probability
    double zTest;

    zTest = (double) rand() / RAND_MAX;

    return (zTest <= p);
}



int gcd(int a, int b) {
    // Ruthlessly stolen from Wikipedia
    int temp;

    while (b != 0) {
        temp = b;
        b = a % b;
        a = temp;
    }

    return a;
}



int min(int a, int b) {
    // Find the smaller of two numbers
    if (a < b) {
        return a;
    } else {
        return b;
    }
}



bool wrap(int* n, int nMin, int nMax) {
    // Keep a number within a specified range; return true if wrap occurs
    if (*n < nMin) {
        *n = nMax;
        return true;
    } else if (*n > nMax) {
        *n = nMin;
        return true;
    } else {
        return false;
    }
}
