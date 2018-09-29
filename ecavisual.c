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
#include "ecaio.h"
#include "allocs.h"
#include "utils.h"

#include "SDL.h"

// TODO: Test with Valgrind memcheck

// Constants

#define WINDOW_TITLE ("ECA Visual")

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
    SDL_Window* windowPtr;
    SDL_Renderer* rendererPtr;
    SDL_Event event;
    Simulation* simPtr;
    Drawing* drawingPtr;
} App;



// Functions for working with rectangle linked lists

RectNode* createNode(Drawing* drawingPtr, int rowIndex, int colIndex) {
    // Create a new RectNode struct
    RectNode* newNodePtr;

    newNodePtr = (RectNode*) safeMalloc(sizeof(RectNode));
    newNodePtr->cellRect = (SDL_Rect) {colIndex * (drawingPtr->xStretch),
                                       rowIndex * (drawingPtr->yStretch),
                                       drawingPtr->xStretch,
                                       drawingPtr->yStretch};
    newNodePtr->colIndex = colIndex;
    newNodePtr->nextNodePtr = (RectNode*) 0;

    return newNodePtr;
}



RectNode* findLastNode(RectNode* intlNodePtr) {
    // Return pointer to last RectNode in a row; return null pointer if list is empty
    RectNode* currentNodePtr;
    RectNode* nextNodePtr;

    if ((currentNodePtr = intlNodePtr) == (RectNode*) 0) {
        return intlNodePtr;
    }

    while ((nextNodePtr = currentNodePtr->nextNodePtr) != (RectNode*) 0) {
        currentNodePtr = nextNodePtr;
    }

    return currentNodePtr;
}



void storeRect(Drawing* drawingPtr, int rowIndex, int colIndex) {
    // Store an SDL_Rect at the specified location in the drawing
    RectNode* newNodePtr;
    RectNode* lastNodePtr;

    RectNode* createNode(Drawing* drawingPtr, int rowIndex, int colIndex);
    RectNode* findLastNode(RectNode* intlNodePtr);

    newNodePtr = createNode(drawingPtr, rowIndex, colIndex);
    lastNodePtr = findLastNode(drawingPtr->rowArr[rowIndex].intlNodePtr);
    if (lastNodePtr == (RectNode*) 0) {
        drawingPtr->rowArr[rowIndex].intlNodePtr = newNodePtr;
    } else {
        lastNodePtr->nextNodePtr = newNodePtr;
    }
}



void storeRow(App* appPtr, int rowIndex) {
    // Create a linked list of SDL_Rects representing a single sim generation
    int colIndex;
    Simulation* simPtr;
    bool rectState;

    extern bool getCellState(CellBlock* intlBlockPtr, int colIndex);
    void storeRect(Drawing* drawingPtr, int rowIndex, int colIndex);

    simPtr = appPtr->simPtr;

    for (colIndex = 0; colIndex < (simPtr->habitatSize); ++colIndex) {
        rectState = getCellState(simPtr->genArr[rowIndex].blockArr, colIndex);
        if (rectState) {
            storeRect(appPtr->drawingPtr, rowIndex, colIndex);
        }
    }
}



void destroyRow(App* appPtr, int rowIndex) {
    // Remove a row of rectangles from memory
    RectNode* currentNodePtr;
    RectNode* nextNodePtr;

    currentNodePtr = appPtr->drawingPtr->rowArr[rowIndex].intlNodePtr;

    while (currentNodePtr != (RectNode*) 0) {
        nextNodePtr = currentNodePtr->nextNodePtr;
        free(currentNodePtr);
        currentNodePtr = nextNodePtr;
    }

    appPtr->drawingPtr->rowArr[rowIndex].intlNodePtr = (RectNode*) 0;
}



void shiftRectsUp(Drawing* drawingPtr, int rowIndex) {
    // Move through a list of rects and shift each one's y coordinate
    RectNode* currentNodePtr;

    if ((currentNodePtr = drawingPtr->rowArr[rowIndex].intlNodePtr) == (RectNode*) 0) {
        return;
    }

    do {
        currentNodePtr->cellRect.y -= drawingPtr->yStretch;
    } while ((currentNodePtr = currentNodePtr->nextNodePtr) != (RectNode*) 0);
}



void shiftRowsUp(App* appPtr) {
    // Move up each rect row in a drawing
    int genBufferSize;
    Drawing* drawingPtr;
    int rowIndex;

    void destroyRow(App* appPtr, int rowIndex);
    void shiftRectsUp(Drawing* drawingPtr, int rowIndex);

    genBufferSize = appPtr->simPtr->genBufferSize;
    drawingPtr = appPtr->drawingPtr;
    // Destroy the first generation - will be overwritten
    destroyRow(appPtr, 0);
    // Shift back the generations that will persist in the buffer
    for (rowIndex = 0; rowIndex < (genBufferSize - 1); ++rowIndex) {
        drawingPtr->rowArr[rowIndex].intlNodePtr = drawingPtr->rowArr[rowIndex + 1].intlNodePtr;
        shiftRectsUp(drawingPtr, rowIndex);
    }
    drawingPtr->rowArr[genBufferSize - 1].intlNodePtr = (RectNode*) 0;
}



// Functions for working with the drawing struct

Drawing* createDrawing(Simulation* simPtr) {
    // Allocate memory to store SDL drawing information
    Drawing* newDrawingPtr;

    newDrawingPtr = (Drawing*) safeMalloc(sizeof(Drawing));
    
    newDrawingPtr->age = 0;
    newDrawingPtr->xStretch = SCREEN_WIDTH / (simPtr->habitatSize);
    newDrawingPtr->yStretch = SCREEN_HEIGHT / (simPtr->genBufferSize);
    newDrawingPtr->background = (SDL_Rect) {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};

    newDrawingPtr->rowArr = (RectRow*) safeMalloc(sizeof(RectRow) * (simPtr->genBufferSize));

    return newDrawingPtr;
}



void initDrawing(App* appPtr) {
    // Erase and redraw the entirety of a visual sim
    int rowIndex;

    extern void iterateSim(Simulation* simPtr, int iterations);
    void destroyRow(App* appPtr, int rowIndex);
    void storeRow(App* appPtr, int rowIndex);

    iterateSim(appPtr->simPtr, (appPtr->simPtr->genBufferSize) - 1);
    for (rowIndex = 0; rowIndex < (appPtr->simPtr->genBufferSize); ++rowIndex) {
        destroyRow(appPtr, rowIndex);
        storeRow(appPtr, rowIndex);
    }
}



void iterateDrawing(App* appPtr) {
    // Update a drawing struct to correspond with an iterated sim
    extern void iterateSim(Simulation* simPtr, int iterations);
    void shiftRowsUp(App* appPtr);
    void storeRow(App* appPtr, int rowIndex);

    iterateSim(appPtr->simPtr, 1);
    shiftRowsUp(appPtr);
    storeRow(appPtr, (appPtr->simPtr->genBufferSize) - 1);
}



void destroyDrawing(App* appPtr) {
    // Remove all of a drawing's data from memory
    int rowIndex;

    void destroyRow(App* appPtr, int rowIndex);

    for (rowIndex = 0; rowIndex < (appPtr->simPtr->genBufferSize); ++rowIndex) {
        destroyRow(appPtr, rowIndex);
    }

    free(appPtr->drawingPtr->rowArr);
    free(appPtr->drawingPtr);
}



// Methods for operating app

App* errorSDL(char* errorMessage) {
    // Write SDL error to terminal
    fprintf(stderr, "%s\n%s\n", errorMessage, SDL_GetError());
    return (App*) 0;
}



App* createApp(Simulation* simPtr, Drawing* drawingPtr) {
    // Create a structure that stores all data for graphical representation
    App* newAppPtr;

    App* errorSDL(char* errorMessage);

    newAppPtr = (App*) safeMalloc(sizeof(App));

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



void destroyApp(App* appPtr) {
    // Remove an entire application's data from memory
    extern void destroySim(Simulation* simPtr);
    void destroyDrawing(App* appPtr);

    destroySim(appPtr->simPtr);
    destroyDrawing(appPtr);
    free(appPtr);
}



void draw(App* appPtr) {
    // Create a visual representation of a simulation
    int rowIndex;
    RectNode* currentNodePtr;

    SDL_RenderClear(appPtr->rendererPtr);
    // Draw background
    SDL_SetRenderDrawColor(appPtr->rendererPtr, BACKGROUND_R,
                                                BACKGROUND_G,
                                                BACKGROUND_B,
                                                BACKGROUND_A);
    SDL_RenderFillRect(appPtr->rendererPtr, &(appPtr->drawingPtr->background));
    // Move through each row and render rectangles representing live cells
    SDL_SetRenderDrawColor(appPtr->rendererPtr, CELL_R,
                                                CELL_G,
                                                CELL_B,
                                                CELL_A);
    for (rowIndex = 0; rowIndex < (appPtr->simPtr->genBufferSize); ++rowIndex) {
        currentNodePtr = appPtr->drawingPtr->rowArr[rowIndex].intlNodePtr;
        while (currentNodePtr != (RectNode*) 0) {
            SDL_RenderFillRect(appPtr->rendererPtr, &(currentNodePtr->cellRect));
            currentNodePtr = currentNodePtr->nextNodePtr;
        }
    }

    SDL_RenderPresent(appPtr->rendererPtr);
}



void resizeSim(Simulation* simPtr, int habitatSize, int genBufferSize) {
    // Resize a simulation that is too large to fit the window
    int genIndex;
    int blockReq;
    CellBlock* currentBlockPtr;

    simPtr->genArr = safeRealloc(simPtr->genArr, sizeof(Generation) * SCREEN_HEIGHT);
    simPtr->genBufferSize = genBufferSize;

    blockReq = (BLOCK_BITS + SCREEN_WIDTH - 1) / BLOCK_BITS; // Ceiling division
    for (genIndex = 0; genIndex < genBufferSize; ++genIndex) {
        currentBlockPtr = simPtr->genArr[genIndex].blockArr;
        simPtr->genArr[genIndex].blockArr = safeRealloc(currentBlockPtr, BLOCK_BYTES * blockReq);
    }
    simPtr->habitatSize = habitatSize;

    fprintf(stderr,
            "Warning: Adjusted simulation size to fit current window\n"
            "Current window size:   %ix%i pixels\n"
            "\n",
            SCREEN_WIDTH, SCREEN_HEIGHT);
}



int operateApp(App* appPtr) {
    // Graphical application logic
    extern void infoStr(Simulation* simPtr);
    void initDrawing(App* appPtr);
    void iterateDrawing(App* appPtr);
    void draw(App* appPtr);
    void destroyApp(App* appPtr);

    infoStr(appPtr->simPtr);
    initDrawing(appPtr);

    while (appPtr->active) {
        draw(appPtr);
        SDL_WaitEvent(&(appPtr->event));
        if (appPtr->event.type == SDL_QUIT) {
            appPtr->active = false;
        } else if (appPtr->event.type == SDL_KEYDOWN) {
            if (appPtr->event.key.keysym.sym == SDLK_SPACE) {
                iterateDrawing(appPtr);
            }
        }
    }
    
    SDL_Quit();
    destroyApp(appPtr);

    return EXIT_SUCCESS;
}



int main(int argc, char* argv[]) {
    // Build all parts of the simulation and then let it loose 
    Simulation* simPtr;
    Drawing* drawingPtr;
    App* appPtr;

    extern Simulation* createUserSim(int argc, char* argv[]);
    void resizeSim(Simulation* simPtr, int habitatSize, int genBufferSize);
    Drawing* createDrawing(Simulation* simPtr);
    App* createApp(Simulation* simPtr, Drawing* drawingPtr);
    int operateApp(App* appPtr);

    simPtr = createUserSim(argc, argv);
    if (simPtr == (Simulation*) 0) {
        return EXIT_FAILURE;
    }

    if (((simPtr->habitatSize) > SCREEN_WIDTH) || ((simPtr->genBufferSize) > SCREEN_HEIGHT)) {
        resizeSim(simPtr, SCREEN_WIDTH, SCREEN_HEIGHT);
    }

    drawingPtr = createDrawing(simPtr);

    appPtr = createApp(simPtr, drawingPtr);
    if (appPtr == (App*) 0) {
        return EXIT_FAILURE;
    }

    return operateApp(appPtr);
}
