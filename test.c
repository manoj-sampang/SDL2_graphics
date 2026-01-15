#include<stdio.h>
#include<SDL2/SDL.h>
#include<stdlib.h>
#include<math.h>

#include "transformation.h"

int choice, n1, n2;
float **arr, **brr;
float sx, sy; 
float theta;
float dx, dy;
int x1, y_1, x2, y_2, x3, y_3;

void input(int n) {
    if(n == 1) {
        
        printf("Input values\n");
        printf("Enter the size of matrix for matrix (A): ");
        scanf("%d", &n1);
        arr = (float**) malloc(n1 * sizeof(float*));
        for(int i = 0; i < n1; ++i) {
            arr[i] = (float*) malloc(n1 * sizeof(float));
            for(int j = 0; j < n1; ++j) {
                printf("A[%d][%d] = ", i, j);
                scanf("%f", &arr[i][j]);        
            
            }
            
        }
        printf("Enter the size of matrix for matrix (B): ");
        scanf("%d", &n2);
        brr = (float**) malloc(n2 * sizeof(float*));
        for(int i = 0; i < n2; ++i) {
            brr[i] = (float*) malloc(n2 * sizeof(float));
            for(int j = 0; j < n2; ++j) {
                printf("B[%d][%d] = ", i, j);
                scanf("%f", &brr[i][j]);       
            }
        }
    }
    if(n == 2) {
        printf("Translation Input\n");
          
        printf("Enter translation distances (dx dy): ");
        scanf("%f %f", &dx, &dy); 
        
    }
    if(n == 3) {
        printf("Rotation Input\n");

        printf("Enter thetha(0) to rotate = ");
        scanf("%f", &theta);

    }
    if(n == 4) {
        printf("Scaling Input\n");
                  
        printf("Enter scaling factors (sx sy): ");  
        scanf("%f %f", &sx, &sy);
        
    }       
    if(n == 5) {
        printf("Triangle Input\n");
        printf("Enter the coordinates of the triangle (x1 y1 x2 y2 x3 y3): ");  
        printf("Point 1(x1, y1): ");
        scanf("%d %d", &x1, &y_1);
        printf("Point 2(x2, y2): ");
        scanf("%d %d", &x2, &y_2);
        printf("Point 3(x3, y3): ");
        scanf("%d %d", &x3, &y_3);
    }   

}

int main() {
    SDL_Init(SDL_INIT_VIDEO);
        
        SDL_Window *window = SDL_CreateWindow(
            "Transformation", 
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            600, 480, 
            0
        );
        SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        
        do{
            printf("=== TRANSFORMATIN ===\n");
            printf("1. MATRIX MULTIPLY\n");
            printf("2. Translate\n");
            printf("3. Rotate\n");
            printf("4. Scale\n");
            printf("5. Draw Traingle\n");
            printf("6. Exit\n");
            printf("Enter choice(1 - 5): ");
            scanf("%d", &choice);
            switch(choice) {
                case 1:
                    printf("--- Matrix Multiply ---\n");
                    input(choice);
                    matrixMultiply(arr, brr, n1);
                    break;
                case 2:
                    printf("--- Translation ---\n");
                    input(choice);
                    translation(dx, dy);
                    break;
                case 3:
                    printf("--- Rotation ---\n");
                    input(choice);
                    rotation(theta);
                    break;

                case 4:
                    printf("--- Scaling ---\n");
                    input(choice);
                    scaling(sx, sy);
                    break;
                case 5:
                    printf("--- Triangle ---\n");
                    input(choice);
                    break;
                case 6:
                    printf("Exiting...\n");
                    for(int i = 0; i < n1; ++i) {
                        free(arr[i]);
                        free(brr[i]);
                    }
                    free(arr);
                    free(brr);
                    SDL_DestroyRenderer(renderer);
                    SDL_DestroyWindow(window);
                    SDL_Quit();
                    exit(0);
                    break;
                default:
                    printf("Invalid Choice\n");
                    break;      
            }
        }while(choice != 5 && choice != 6);
    
            
            
            
            
            int running = 1;
            SDL_Event event;
            while(running) {
                while(SDL_PollEvent(&event)) {
                    if(event.type == SDL_QUIT)
                    running = 0;
                }
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                SDL_RenderClear(renderer);
                if(choice == 4) {
                    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                    drawTriangle(renderer, x1, y_1, x2, y_2, x3, y_3);
                }
                SDL_RenderPresent(renderer);
            }
        for(int i = 0; i < n1; ++i) {
                free(arr[i]);
                free(brr[i]);
        }
        free(arr);
        free(brr);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 0;
    }