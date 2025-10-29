#include <iostream>
#include <stack>
#include <SDL3/SDL.h>
#include <cmath>
#include <fstream>

uint8_t memory[4096] = {0};
uint8_t V[16]; // registers v0 throught vF
uint16_t I{0}; // memory addresses register
uint8_t delay{0}; // delay register
uint8_t sound{0}; // sound register
uint16_t PC{0}; // program counter
std::stack<uint16_t> stack; // stores the address that the PC will be set to when finishing a subroutine
uint8_t* SP = nullptr; // stack pointer

int import_program() {
	std::ifstream program("C:\\Chip-8 pragrams\\2-ibm-logo.ch8", std::ios::binary);
	if(!program.is_open()) {
		std::cerr << "could not open file!";
		return -1;
	}
	program.read(reinterpret_cast<char*>(memory + 0x200), 3584);
	PC = 0x200;
	return 0;
}


void chip_8_cycle() {

}


int main() {

	if(import_program() == -1) return -1;

	SDL_INIT_VIDEO;
	SDL_Window* window = SDL_CreateWindow("chip-8", 640, 320, SDL_WINDOW_RESIZABLE);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr);
	bool buffer[64][32] = {0};

	bool running{true};
	SDL_Event event;
	while(running) {
		while(SDL_PollEvent(&event)) {
			if(event.type == SDL_EVENT_QUIT) running = false;
		}

		chip_8_cycle();

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		for(int y = 0; y < 32; y++) {
			for(int x = 0; x < 64; x++) {
				if(buffer[y][x]) {

					SDL_FRect pixel{x * 10.f, y * 10.f, 10.f, 10.f};
					SDL_RenderFillRect(renderer, &pixel);
				}
			}
		}
		SDL_RenderPresent(renderer);
		SDL_Delay(16.667);
	}
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}
