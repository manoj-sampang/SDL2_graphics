#include<SDL2/SDL.h>
#include "src/ownsdl.h"

void drawCircle(SDL_Renderer *renderer, int xc, int yc, int r) {
    int x = 0, y = r;
    float P = 1.25f - r;
    SDL_RenderDrawPoint(renderer, xc, yc);
    while (x <= y) {
        SDL_RenderDrawPoint(renderer, xc + x, yc + y);
        SDL_RenderDrawPoint(renderer, xc - x, yc + y);
        SDL_RenderDrawPoint(renderer, xc + y, yc + x);
        SDL_RenderDrawPoint(renderer, xc - y, yc + x);
        SDL_RenderDrawPoint(renderer, xc + x, yc - y);
        SDL_RenderDrawPoint(renderer, xc - x, yc - y);
        SDL_RenderDrawPoint(renderer, xc + y, yc - x);
        SDL_RenderDrawPoint(renderer, xc - y, yc - x);

        if (P < 0) {
            P += 2 * x + 3;
        } else {
            P += 2 * (x - y) + 5;
            y--;
        }
        x++;
    }
    DDA(renderer, xc, yc, xc + r, yc);
}
