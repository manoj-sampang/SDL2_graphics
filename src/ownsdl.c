
#include<SDL2/SDL.h>
#include<math.h>

void DDA(SDL_Renderer *renderer, int x0, int y0, int x1, int y1) {
    int steps, dx, dy;
    float x, y;
    dx = x1 - x0;
    dy = y1 - y0;
    x = x0;
    y = y0;

    steps = abs(dx) > abs(dy) ? abs(dx) : abs(dy);
    float x_inr, y_inr;

    x_inr = dx / (float)steps;
    y_inr = dy / (float)steps;

    for(int i = 0; i <= steps; ++i) {
        SDL_RenderDrawPoint(renderer, round(x), round(y));
        x += x_inr;
        y += y_inr;
        
    }
    
}


void BRESENHAM(SDL_Renderer* renderer, int x0, int y0, int x1, int y1) {
    int dx, dy, x, y;
    int p;
    dx = x1 - x0;
    dy = y1 - y0;
    p = 2*(dy - dx);
    x = x0;
    y = y1;
    SDL_RenderDrawPoint(renderer, x, y);
    while(x < x1) {
        x++;
        if(p < 0) {
            p +=2*dy;
        }
        else {
            y++;
            p += 2*(dy - dx);
        }
        SDL_RenderDrawPoint(renderer, x, y);
    }
}