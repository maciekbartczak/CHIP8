#include "chip8.h"
#include <iostream>
#include <SDL.h>
#include <chrono>
#include <thread>

const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 768;

using namespace std;

const int keymap[] = {
	SDLK_1, SDLK_2, SDLK_3, SDLK_4,
	SDLK_q, SDLK_w, SDLK_e, SDLK_r,
	SDLK_a, SDLK_s, SDLK_d, SDLK_f,
	SDLK_z, SDLK_x, SDLK_c, SDLK_v
};

int main(int argc, char** argv) {
	if (argc < 2) {
		cout << "usage: CHIP8 <filename>" << endl;
		return 1;
	}
	Chip8 chip8;
	if (chip8.loadROM(argv[1]) == false) {
		return 2;
	}

	SDL_Window* window = NULL;
	SDL_Renderer* renderer = NULL; 
	SDL_Texture* texture = NULL;
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		cout << "SDL_Init error " << SDL_GetError() << endl;
		return 3;
	}
	window = SDL_CreateWindow("CHIP8", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
							  SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (window == NULL) {
		cout << "SDL_CreateWindow error " << SDL_GetError() << endl;
		return 3;
	}
	renderer = SDL_CreateRenderer(window, -1, 0);
	texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB332,
			  SDL_TEXTUREACCESS_STATIC, 64, 32);

	SDL_Event event;
	bool run = true;
	while (run) {
		chip8.run_cycle();
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_QUIT:
					run = false;
					break;
				case SDL_KEYDOWN:
					for (int i = 0; i < 16; i++) {
						if (event.key.keysym.sym == keymap[i]) {
							chip8.keypad[i] = true;
						}
					}
					break;
				case SDL_KEYUP:
					for (int i = 0; i < 16; i++) {
						if (event.key.keysym.sym == keymap[i]) {
							chip8.keypad[i] = false;
						}
					}
			}
		}
		if (chip8.draw) {
			chip8.draw = false;
			SDL_UpdateTexture(texture, NULL,chip8.gfx_mem,64*sizeof(uint8_t));
			SDL_RenderClear(renderer);
			SDL_RenderCopy(renderer, texture, NULL, NULL);
			SDL_RenderPresent(renderer);
		}
		chip8.decrement_timers();
		std::this_thread::sleep_for(std::chrono::microseconds(100));
	}
	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}