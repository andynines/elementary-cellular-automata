/*
ecavisual.c
Copyright (c) 2018 andynines
MIT License
*/

// Includes

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "eca.h"
#include "ecaargs.h"
#include "ecaout.h"

#include "SDL.h"



// Constants

#define WINDOW_TITLE ("Elementary Cellular Automata Simulator")

#define SCREEN_WIDTH (480)
#define SCREEN_HEIGHT (360)

#define BACKGROUND_R (0xFF)
#define BACKGROUND_G (0xFF)
#define BACKGROUND_B (0xFF)
#define BACKGROUND_A (0xFF)
#define CELL_R (0x0)
#define CELL_G (0x0)
#define CELL_B (0x0)
#define CELL_A (0xFF)



// Data structures

struct rectNode {
    SDL_Rect cellRect;
    int colIndex;
    struct rectNode* nextNodePtr;
};
typedef struct rectNode RectNode;

typedef struct {
    RectNode* intlNodePtr;
} RectRow;

typedef struct {
    int age;
    int xStretch;
    int yStretch;
    SDL_Rect background;
    RectRow* rowArr;
} Drawing;

typedef struct {
    bool active;
    bool bufferFilled;
    SDL_Window* windowPtr;
    SDL_Renderer* rendererPtr;
    SDL_Event event;
    Simulation* simPtr;
    Drawing* drawingPtr;
} App;



// Methods for working with data structures

Drawing* createDrawing(Simulation* simPtr) {
    // Allocate memory to store SDL drawing information
    Drawing* newDrawingPtr;

    newDrawingPtr = (Drawing*) malloc(sizeof(Drawing));
    //TODO: Change method of working with ecaargs so args are specified by caller
    newDrawingPtr->xStretch = SCREEN_WIDTH / (simPtr->habitatSize);
    newDrawingPtr->yStretch = SCREEN_HEIGHT / (simPtr->genBufferSize);

    newDrawingPtr->background = (SDL_Rect) {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};

    newDrawingPtr->rowArr = (RectRow*) malloc(sizeof(RectRow) * (simPtr->genBufferSize));

    return newDrawingPtr;
}



void updateVisual(Simulation* simPtr, Drawing* drawingPtr) {
    // Update a drawing struct to correspond with a sim
    int ageDiff;
    int rowIndex;

    ageDiff = (simPtr->age) - (drawingPtr->age);
    if (ageDiff == 0) {
        return;
    } else if ((drawingPtr->age) > (simPtr->genBufferSize)) {
        // shift back rectrows agediff tiems
    }

    rowIndex = (simPtr->genBufferSize) - ageDiff - 1;
    for (; rowIndex < (simPtr->genBufferSize); ++rowIndex) {
    
    }
}



// Methods for operating app

void draw(App* appPtr) {
    // Create a visual representation of a simulation
    SDL_RenderClear(appPtr->rendererPtr);
    SDL_SetRenderDrawColor(appPtr->rendererPtr, BACKGROUND_R,
                                                BACKGROUND_G,
                                                BACKGROUND_B,
                                                BACKGROUND_A);
    SDL_RenderFillRect(appPtr->rendererPtr, &(appPtr->drawingPtr->background));

    SDL_RenderPresent(appPtr->rendererPtr);
}



App* errorSDL(char* errorMessage) {
    // Write SDL error to terminal
    fprintf(stderr, "%s\n%s\n", errorMessage, SDL_GetError());
    return (App*) 0;
}



App* createApp(Simulation* simPtr, Drawing* drawingPtr) {
    // Create a structure that stores all data for graphical representation
    App* newAppPtr;

    App* errorSDL(char* errorMessage);

    newAppPtr = (App*) malloc(sizeof(App));

    newAppPtr->active = true;
    newAppPtr->windowPtr = NULL;
    newAppPtr->rendererPtr = NULL;
    newAppPtr->simPtr = simPtr; 
    newAppPtr->drawingPtr = drawingPtr;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        return errorSDL("Could not initialize");
    }
    newAppPtr->windowPtr = SDL_CreateWindow(WINDOW_TITLE,
                                            SDL_WINDOWPOS_UNDEFINED,
                                            SDL_WINDOWPOS_UNDEFINED,
                                            SCREEN_WIDTH,
                                            SCREEN_HEIGHT,
                                            SDL_WINDOW_SHOWN);
    if (newAppPtr->windowPtr == NULL) {
        return errorSDL("Could not create window");
    }
    newAppPtr->rendererPtr = SDL_CreateRenderer(newAppPtr->windowPtr, -1, SDL_RENDERER_ACCELERATED);
    if (newAppPtr->rendererPtr == NULL) {
        return errorSDL("Could not create renderer");
    }

    return newAppPtr;
}



int operateApp(App* appPtr) {
    // Graphical application logic
    extern void infoStr(Simulation* simPtr);
    void updateVisual(Simulation* simPtr, Drawing* drawingPtr);
    void draw(App* appPtr);

    infoStr(appPtr->simPtr);
    while (appPtr->active) {
        while (SDL_PollEvent(&(appPtr->event)) != 0) {
            updateVisual(appPtr->simPtr, appPtr->drawingPtr);
            draw(appPtr);
            if (appPtr->event.type == SDL_QUIT) {
                appPtr->active = false;
            }

        }
    }
    
    SDL_Quit();

    return EXIT_SUCCESS;
}



int main(int argc, char* argv[]) {
    // Build all parts of the simulation and then let it loose 
    Simulation* simPtr;
    Drawing* drawingPtr;
    App* appPtr;

    extern Simulation* createUserSim(int argc, char* argv[]);
    Drawing* createDrawing(Simulation* simPtr);
    App* createApp(Simulation* simPtr, Drawing* drawingPtr);
    int operateApp(App* appPtr);

    simPtr = createUserSim(argc, argv);
    if (simPtr == (Simulation*) 0) {
        return EXIT_FAILURE;
    }

    drawingPtr = createDrawing(simPtr);

    appPtr = createApp(simPtr, drawingPtr);
    if (appPtr == (App*) 0) {
        return EXIT_FAILURE;
    }

    return operateApp(appPtr);
}
