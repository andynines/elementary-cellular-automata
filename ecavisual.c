/*
ecavisual.c
Copyright (c) 2018 andynines
MIT License
*/

// Includes

#include <stdbool.h>
#include <stdio.h>

#include "SDL.h"



// Constants

#define SCREEN_WIDTH (480)
#define SCREEN_HEIGHT (360)



// Creating window

void showError(char* errorMessage) {
    printf("%s\n%s\n", errorMessage, SDL_GetError());
}



int main(int argc, char* args[]) {
   
    bool active;
    SDL_Event eventHandler;
    SDL_Window* window;
    SDL_Surface* screenSurface;

    active = true;
    window = NULL;
    screenSurface = NULL;
    
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        showError("Could not initialize");
    } else {
        window = SDL_CreateWindow("Elementary Cellular Automata Simulator",
                                  SDL_WINDOWPOS_UNDEFINED,
                                  SDL_WINDOWPOS_UNDEFINED,
                                  SCREEN_WIDTH,
                                  SCREEN_HEIGHT,
                                  SDL_WINDOW_SHOWN);
        if (window == NULL) {
            showError("Could not create window");
        } else {
            screenSurface = SDL_GetWindowSurface(window);
            SDL_FillRect(screenSurface,
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
            }
        }
    }
    
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
