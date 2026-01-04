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