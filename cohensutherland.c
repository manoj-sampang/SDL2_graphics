#include <stdio.h>
#include <unistd.h>
#include <SDL2/SDL.h>
//implemntation of cohen Sutherland Line Clipping Algorithm
void clear_screen() {
	#ifdef _WIN32 {
		system("cls");
	}
	#else {
		system("clear");
	}
	#endif
}
int main() {
	if (SDL_Init(SDL_INIT_VIDEO) != 0 ) {
		printf("SDL error: %s\n", SDL_GetError());
		return 1;
	}	
	SDL_Window *window = SDL_CreateWindow(
		"Cohen Line Clipping",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		600, 400,
		0
	);
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	int running = 1;
	SDL_Event event;
	
	while(running) 	{
		while(SDL_PollEvent(&Event)) {
			if(event.type == SDL_QUIT)
				running = 0;
		}
		
	}
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0; 
}
