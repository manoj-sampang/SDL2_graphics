#include<stdio.h>
#include<SDL2/SDL.h>
#include "ownsdl.h"

int n0, m0, n1, m1;


void input() {
    printf("=== DDA Line Algorithm ===\n");
    printf("INPUT END POINTS OF THE LINE\n");
    printf("(x0, y0) = ");
    scanf("%d, %d", &n0, &m0);
    printf("(x1, y1) = ");
    scanf("%d, %d", &n1, &m1);
}
int main() {
    //SDL open/ initialization
    input();
    SDL_Init(SDL_INIT_VIDEO);

    //SDL creating a window of width 450 and height 600
    SDL_Window *window = SDL_CreateWindow(
        "BRESENHAM LINE",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        600, 450, 
        0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    //event handling loop
    int running = 1;
    SDL_Event event;

    while(running) {
        while(SDL_PollEvent(&event)) {
            if(event.type == SDL_QUIT)
                running = 0;
        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
    
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    
        BRESENHAM(renderer, n0, m0, n1, m1);
    
        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;

        
}