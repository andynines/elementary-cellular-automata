/*
utils.h
Copyright (c) 2018 andynines
MIT License
*/

#ifndef UTILS_H

#define UTILS_H



// Includes

#include <stdbool.h>



// Fraction data type

typedef struct {
    int num;
    int denom;
} Fraction;



// Function declarations

Fraction* createFrac(int num, int denom);

void simplifyFrac(Fraction* fracPtr);

void destroyFrac(Fraction* fracPtr);

bool binomProb(double p);

int min(int a, int b);

bool wrap(int* n, int nMax);



#endif
