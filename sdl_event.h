#pragma once
#include <SDL3/SDL.h>

extern bool key_pressed[16];
extern SDL_Event event;
extern bool running;

void sdl_event() {
	while(SDL_PollEvent(&event)) {
		if(event.type == SDL_EVENT_QUIT) {
			running = false;
		}
		else if(event.type == SDL_EVENT_KEY_DOWN) {

			SDL_Keycode key = event.key.key;
			switch(key) {
				case SDLK_1: key_pressed[0x1] = 1; break;
				case SDLK_2: key_pressed[0x2] = 1; break;
				case SDLK_3: key_pressed[0x3] = 1; break;
				case SDLK_4: key_pressed[0xC] = 1; break;
				case SDLK_Q: key_pressed[0x4] = 1; break;
				case SDLK_W: key_pressed[0x5] = 1; break;
				case SDLK_E: key_pressed[0x6] = 1; break;
				case SDLK_R: key_pressed[0xD] = 1; break;
				case SDLK_A: key_pressed[0x7] = 1; break;
				case SDLK_S: key_pressed[0x8] = 1; break;
				case SDLK_D: key_pressed[0x9] = 1; break;
				case SDLK_F: key_pressed[0xE] = 1; break;
				case SDLK_Z: key_pressed[0xA] = 1; break;
				case SDLK_X: key_pressed[0x0] = 1; break;
				case SDLK_C: key_pressed[0xB] = 1; break;
				case SDLK_ESCAPE:  running = false;  break;
			}
		}
		else if(event.type == SDL_EVENT_KEY_UP) {

			SDL_Keycode key = event.key.key;
			switch(key) {
				case SDLK_1: key_pressed[0x1] = 0; break;
				case SDLK_2: key_pressed[0x2] = 0; break;
				case SDLK_3: key_pressed[0x3] = 0; break;
				case SDLK_4: key_pressed[0xC] = 0; break;
				case SDLK_Q: key_pressed[0x4] = 0; break;
				case SDLK_W: key_pressed[0x5] = 0; break;
				case SDLK_E: key_pressed[0x6] = 0; break;
				case SDLK_R: key_pressed[0xD] = 0; break;
				case SDLK_A: key_pressed[0x7] = 0; break;
				case SDLK_S: key_pressed[0x8] = 0; break;
				case SDLK_D: key_pressed[0x9] = 0; break;
				case SDLK_F: key_pressed[0xE] = 0; break;
				case SDLK_Z: key_pressed[0xA] = 0; break;
				case SDLK_X: key_pressed[0x0] = 0; break;
				case SDLK_C: key_pressed[0xB] = 0; break;
			}
		}
	}
}
