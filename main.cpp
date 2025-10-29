#include <array>
#include <stack>
#include <SDL3/SDL.h>
#include <cmath>

void decode_program() {


}


void chip_8_cycle() {




}


int main() {

	std::array<uint8_t, 4096> memory = {0};
	uint8_t V[16];
	uint16_t I{0};
	uint8_t delay{0};
	uint8_t sound{0};
	uint16_t PC{0};
	std::stack<uint16_t> stack;
	uint8_t* SP = nullptr;

	decode_program();

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
