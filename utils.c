/*
utils.c
Copyright (c) 2018 andynines
MIT License
*/

// Includes

#include <stdbool.h>
#include <stdlib.h>

#include "utils.h"



// Functions for working with fractions TODO: make separate lib

static int gcd(int a, int b) {
    // Ruthlessly stolen from Wikipedia
    int temp;

    while (b != 0) {
        temp = b;
        b = a % b;
        a = temp;
    }

    return a;
}



Fraction* createFrac(int num, int denom) {
    // Create a new fraction structure
    Fraction* newFracPtr;

    newFracPtr = (Fraction*) malloc(sizeof(Fraction));
    newFracPtr->num = num;
    newFracPtr->denom = denom;

    return newFracPtr;
}



void simplifyFrac(Fraction* fracPtr) {
    // Reduce a fraction to the simplest numbers that make its ratio
    int currentGcd;

    int gcd(int a, int b);

    while ((currentGcd = gcd(fracPtr->num, fracPtr->denom)) > 1) {
        fracPtr->num /= currentGcd;
        fracPtr->denom /= currentGcd;
    }
}



void destroyFrac(Fraction* fracPtr) {
    // Remove a fraction from memory
    free(fracPtr);
}



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
    // Keep a number within a specified range; return true if wrap occurs
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
