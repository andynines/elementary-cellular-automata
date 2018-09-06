/*
ecaio.h
Copyright (c) 2018 andynines
MIT License
*/

#ifndef ECAIO_H

#define ECAIO_H



// Includes

#include "eca.h"



// Constants

#define TOTAL_ARGC (7)
#define ECA_ARGC (TOTAL_ARGC - 1)



// Available output characters

typedef enum {
    DEAD_CELL = '-',
    ALIVE_CELL = '0'
} OutChar;



// Function declarations

Simulation* createUserSim(int argc, char* argv[]);

void infoStr(Simulation* simPtr);

void genOut(Simulation* simPtr, int genIndex); 

void simOut(Simulation* simPtr);



#endif
