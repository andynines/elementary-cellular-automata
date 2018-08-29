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

#include "SDL.h"



// Creating window

int showError(char* errorMessage) {
    // Write SDL error to terminal
    fprintf(stderr, "%s\n%s\n", errorMessage, SDL_GetError());
    return EXIT_FAILURE;
}



int main(int argc, char* argv[]) {
  
    Simulation* simPtr;
    int screenWidth;
    int screenHeight;
    bool active;
    SDL_Window* window;
    SDL_Surface* screenSurface;
    SDL_Renderer* renderer;
    SDL_Event eventHandler;

    extern Simulation* createUserSim(int argc, char* argv[]);
    extern void iterateSim(Simulation* simPtr, int iterations);
    extern void destroySim(Simulation* simPtr);

    simPtr = createUserSim(argc, argv);
    if (simPtr == (Simulation*) 0) {
        return EXIT_FAILURE;
    }
    screenWidth = simPtr->habitatSize;
    screenHeight = simPtr->genBufferSize;
    active = true;
    window = NULL;
    screenSurface = NULL;
    renderer = NULL;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        return showError("Could not initialize");
    } else {
        window = SDL_CreateWindow("Elementary Cellular Automata Simulator",
                                  SDL_WINDOWPOS_UNDEFINED,
                                  SDL_WINDOWPOS_UNDEFINED,
                                  screenWidth,
                                  screenHeight,
                                  SDL_WINDOW_SHOWN);
        if (window == NULL) {
            return showError("Could not create window");
        } else {
            screenSurface = SDL_GetWindowSurface(window);
            SDL_FillRect(screenSurface, //needed?
                         NULL,
                         SDL_MapRGB(screenSurface->format,
                                    0xFF, 0xFF, 0xFF));
            SDL_UpdateWindowSurface(window);
            while (active) {
                while (SDL_PollEvent(&eventHandler) != 0) {
                    if (eventHandler.type == SDL_QUIT) {
                        active = false;
                    }
                }
                SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
                SDL_RenderClear(renderer);
            }
        }
    }
    
    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;
}
