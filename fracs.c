/*
fracs.c
Copyright (c) 2018 andynines
MIT License
*/

// Includes

#include <stdlib.h>

#include "allocs.h"
#include "fracs.h"



// Functions for working with fractions

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

    newFracPtr = (Fraction*) safeMalloc(sizeof(Fraction));
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
