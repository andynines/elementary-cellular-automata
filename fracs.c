/*
fracs.c

Methods for working with structures meant to represent mathematical fractions.

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
    /*
    Given a numerator and denominator, allocate the space to store these ints in
    a fraction "object" for further calculations. Return a pointer to the new 
    structure.
    */
    Fraction* newFracPtr;

    newFracPtr = (Fraction*) safeMalloc(sizeof(Fraction));
    newFracPtr->num = num;
    newFracPtr->denom = denom;

    return newFracPtr;
}



void simplifyFrac(Fraction* fracPtr) {
    /*
    Given a pointer to a fraction structure, reduce its numerator and 
    denominator down to their simplest form.
    */
    int currentGcd;

    int gcd(int a, int b);

    while ((currentGcd = gcd(fracPtr->num, fracPtr->denom)) > 1) {
        fracPtr->num /= currentGcd;
        fracPtr->denom /= currentGcd;
    }
}



void destroyFrac(Fraction* fracPtr) {
    /*
    Wrap the standard deallocation method for completeness. Free the fraction
    structure pointed to by the argument.
    */
    free(fracPtr);
}
