//.c file of header file transformatin.h
#include<SDL2/SDL.h>
#include<math.h>
#include<stdio.h>
#include "src/ownsdl.h"

void matrixMultiply(float **a, float **b, int n)
{
    float **result = malloc(n * sizeof(float*));
    for(int i = 0; i < n; i++)
        result[i] = malloc(n * sizeof(float));

    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            result[i][j] = 0;
            for(int k = 0; k < n; k++) {
                result[i][j] += a[i][k] * b[k][j];
            }
        }
    }

    printf("Result Matrix:\n");
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++)
            printf("%.2f ", result[i][j]);
        printf("\n");
    }
    for(int i = 0; i < n; i++)
        free(result[i]);
    free(result);
}

void translation(float dx, float dy) {
        //translation matrix
        float translationMatrix[3][3] = {
            {1, 0, dx},
            {0, 1, dy},
            {0, 0, 1}
        };
        printf("Translation Matrix:\n");
        for(int i = 0; i < 3; ++i) {    
            for(int j = 0; j < 3; ++j) {
                printf("%f ", translationMatrix[i][j]);
            }
            printf("\n");
        }   
}
void rotation(float theta) {
    float rotateMatrix[3][3] = {
        {cos(theta), -sin(theta), 0},
        {sin(theta), cos(theta), 0},
        {0, 0, 1}
    };
    printf("Rotated Matrix:\n");
    for(int i = 0; i < 3; ++i) {
        for(int j = 0; j < 3; ++j) {            
            printf("%f ", rotateMatrix[i][j]);
        }
        printf("\n"); 
    }
}
void scaling(float sx, float sy) {
    //scaling matrix
    float scalingMatrix[3][3] = {
        {sx, 0, 0},
        {0, sy, 0},
        {0, 0, 1}
    };
    printf("Scaling Matrix:\n");
    for(int i = 0; i < 3; ++i) {    
        for(int j = 0; j < 3; ++j) {            
            printf("%f ", scalingMatrix[i][j]);
        }
        printf("\n"); 
    }
}
void drawTriangle(SDL_Renderer *renderer, int x1, int y1, int x2, int y2, int x3, int y3) {
    DDA(renderer, x1, y1, x2, y2);
    DDA(renderer, x2, y2, x3, y3);
    DDA(renderer, x3, y3, x1, y1);
}


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
//drawing two rectangles and and viewport transformation is done of the triangle

void drawRectangle(SDL_Renderer *renderer, int xmin, int ymin, int xmax, int ymax) {
    DDA(renderer, xmin, ymin, xmax, ymin);  // Top
    DDA(renderer, xmax, ymin, xmax, ymax);  // Right
    DDA(renderer, xmax, ymax, xmin, ymax);  // Bottom
    DDA(renderer, xmin, ymax, xmin, ymin);  // Left
}

void windowToViewPort(int xw, int yw, int xwmin, int ywmin, int xwmax, int ywmax, 
                      int xvmin, int yvmin, int xvmax, int yvmax, int *xv, int *yv) {
    // Normalize to window coordinates [0, 1]
    float sx = (float)(xw - xwmin) / (xwmax - xwmin);
    float sy = (float)(yw - ywmin) / (ywmax - ywmin);
    
    // Transform to viewport coordinates
    *xv = xvmin + (int)(sx * (xvmax - xvmin));
    *yv = yvmin + (int)(sy * (yvmax - yvmin));
}

void drawTransformedTriangle(SDL_Renderer *renderer, 
                             int xwmin, int ywmin, int xwmax, int ywmax,
                             int xvmin, int yvmin, int xvmax, int yvmax) {
    // Triangle vertices in window coordinates (upside down with equal spacing)
    int x1w = (xwmin + xwmax) / 2;  // Top center
    int y1w = ywmin + 20;
    
    int x2w = xwmin + 20;           // Bottom left
    int y2w = ywmax - 20;
    
    int x3w = xwmax - 20;           // Bottom right
    int y3w = ywmax - 20;
    
    // Transform to viewport coordinates
    int x1v, y1v, x2v, y2v, x3v, y3v;
    windowToViewPort(x1w, y1w, xwmin, ywmin, xwmax, ywmax, xvmin, yvmin, xvmax, yvmax, &x1v, &y1v);
    windowToViewPort(x2w, y2w, xwmin, ywmin, xwmax, ywmax, xvmin, yvmin, xvmax, yvmax, &x2v, &y2v);
    windowToViewPort(x3w, y3w, xwmin, ywmin, xwmax, ywmax, xvmin, yvmin, xvmax, yvmax, &x3v, &y3v);
    
    // Draw transformed triangle
    drawTriangle(renderer, x1v, y1v, x2v, y2v, x3v, y3v);
}

// Main demonstration function
// Main demonstration function
void demonstrateWindowToViewPort(SDL_Renderer *renderer) {
    // Large rectangle (window) - LEFT SIDE
    int xwmin = 50, ywmin = 100, xwmax = 350, ywmax = 400;
    
    // Small rectangle (viewport) - RIGHT SIDE
    int xvmin = 450, yvmin = 150, xvmax = 650, yvmax = 350;
    
    // Set color to white
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    
    // Draw large rectangle (window)
    drawRectangle(renderer, xwmin, ywmin, xwmax, ywmax);
    
    // Draw small rectangle (viewport)
    drawRectangle(renderer, xvmin, yvmin, xvmax, yvmax);
    
    // Triangle vertices in window coordinates (upside down with equal spacing inside large rectangle)
    int x1w = xwmin + 30;               // Top left with spacing
    int y1w = ywmin + 30;               // Top with spacing
    
    int x2w = xwmax - 30;               // Top right with spacing
    int y2w = ywmin + 30;               // Top with spacing
    
    int x3w = (xwmin + xwmax) / 2;      // Bottom center (apex pointing down)
    int y3w = ywmax - 30;         
    // Draw upside-down triangle in large rectangle
    drawTriangle(renderer, x1w, y1w, x2w, y2w, x3w, y3w);
    
    // Transform triangle vertices to viewport coordinates
    int x1v, y1v, x2v, y2v, x3v, y3v;
    windowToViewPort(x1w, y1w, xwmin, ywmin, xwmax, ywmax, xvmin, yvmin, xvmax, yvmax, &x1v, &y1v);
    windowToViewPort(x2w, y2w, xwmin, ywmin, xwmax, ywmax, xvmin, yvmin, xvmax, yvmax, &x2v, &y2v);
    windowToViewPort(x3w, y3w, xwmin, ywmin, xwmax, ywmax, xvmin, yvmin, xvmax, yvmax, &x3v, &y3v);
    
    // Draw transformed triangle in small rectangle (viewport)
    drawTriangle(renderer, x1v, y1v, x2v, y2v, x3v, y3v);
}