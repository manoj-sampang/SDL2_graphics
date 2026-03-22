#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>


#define WINDOW_TITLE "GRAPHICS"
#define WIDTH 600
#define HEIGHT 800

//learning SDL2 basics in C 
//before any graphics components first we need to create a window where we can see the content of the pixels graphed.
//and also create a renderer that allows us to draw in the window for us to see


// functions prototype to handle SDL initialization
bool SDL_initialize(SDL_Renderer*, SDL_Window*);
void window_close(SDL_Renderer*, SDL_Window*);
int main() {
	SDL_Renderer* renderer = NULL;
	SDL_Window* window = NULL;
	if(!SDL_initialize(renderer, window)) {
		window_close(renderer, window);
	}
	return 0;
}

bool SDL_initialize(SDL_Renderer* renderer, SDL_Window* window) {
	window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0
	);
	
	renderer = SDL_CreateRenderer(window, -1, 0);
	if(renderer || window == NULL) {
		return true;
	}
	SDL_Delay(5000);
	return false;
}

void window_close(SDL_Renderer* renderer, SDL_Window* window) {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}
