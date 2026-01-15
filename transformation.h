
// functions to implement
// 3 * 3 matrix multiplication
// translate function
//scaling
// draw traingle
#ifndef TRANSFORMATION_H
#define TRANSFORMATION_H

void matrixMultiply(float **, float **, int);
void translation(float, float);
void rotation(float);
void scaling(float, float);
void drawTriangle(SDL_Renderer *, int, int, int, int, int, int);
void DDA(SDL_Renderer *, int, int, int, int);


#endif