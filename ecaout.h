/*
ecaout.h
Copyright (c) 2018 andynines
MIT License
*/

#ifndef ECAOUT_H

#define ECAOUT_H



// Includes

#include "eca.h"



// Available output characters

typedef enum {
    DEAD_CELL = '-',
    ALIVE_CELL = '0'
} OutChar;



// Function declarations

void genOut(Simulation* simPtr, int genIndex);

void simOut(Simulation* simPtr);



#endif
