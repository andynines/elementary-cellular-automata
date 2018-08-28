/*
fracs.h
Copyright (c) 2018 andynines
MIT License
*/

#ifndef FRACS_H

#define FRACS_H



// Fraction data type

typedef struct {
    int num;
    int denom;
} Fraction;



// Function declarations

Fraction* createFrac(int num, int denom);

void simplifyFrac(Fraction* fracPtr);

void destroyFrac(Fraction* fracPtr);



#endif
