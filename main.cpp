#include <iostream>
#include <stack>
#include <SDL3/SDL.h>
#include <cmath>
#include <fstream>
#include <cstdlib>
#include <ctime> 

uint8_t memory[4096] = {0};
uint8_t V[16]; // registers v0 throught vF
uint16_t I{0}; // memory addresses register
uint8_t delay{0}; // delay register
uint8_t sound{0}; // sound register
uint16_t PC{0}; // program counter
std::stack<uint16_t> stack; // stores the address that the PC will be set to when finishing a subroutine
bool sreen_buffer[64][32] = {0}; // the screen state

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
	uint16_t opcode = (memory[PC] << 8) | (memory[PC + 1]);

	switch(opcode & 0xF000) {
		case 0x0000:
			switch(opcode & 0x00FF) {
				case 0x00E0: // CLS
					std::memset(sreen_buffer, 0, std::size(sreen_buffer));
					break;
				case 0x00EE: // RET
					PC = stack.top();
					stack.pop();
					break;
				default: // SYS addr (ignored)
					break;
			}
			PC += 2;
		case 0x1000: // JP addr
			PC = opcode & 0x0FFF;
			break;
		case 0x2000: // CALL addr
			stack.push(PC);
			PC = opcode & 0x0FFF;
			PC++;
			break;
		case 0x3000: // SE Vx, byte
			if(V[(opcode & 0x0F00) >> 8] == (opcode & 0x00FF)) PC += 4;
			break;
		case 0x4000: // SNE Vx, byte
			if(V[(opcode & 0x0F00) >> 8] != (opcode & 0x00FF)) PC += 4;
			break;
		case 0x5000: // SE Vx, Vy
			if(V[(opcode & 0x0F00) >> 8] == V[(opcode & 0x00F0) >> 4]) PC += 4;
			break;
		case 0x6000: // LD Vx, byte
			V[opcode & (0x0F00 >> 8)] = (opcode & 0x00FF);
			PC += 2;
			break;
		case 0x7000: // ADD Vx, byte
			V[opcode & (0x0F00 >> 8)] += (opcode & 0x00FF);
			PC++;
			break;
		case 0x8000:
			switch(opcode & 0x000F) {
				case 0x0000: // LD Vx, Vy
					V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4];
					break;
				case 0x0001: // OR Vx, Vy
					V[(opcode & 0x0F00) >> 8] |= V[(opcode & 0x00F0) >> 4];
					break;
				case 0x0002: // AND Vx, Vy
					V[(opcode & 0x0F00) >> 8] &= V[(opcode & 0x00F0) >> 4];
					break;
				case 0x0003: // XOR Vx, Vy
					V[(opcode & 0x0F00) >> 8] ^= V[(opcode & 0x00F0) >> 4];
					break;
				case 0x0004: // ADD Vx, Vy
					V[0xF] = (V[(opcode & 0x0F00) >> 8] + V[(opcode & 0x00F0) >> 4]) > 0xFF;
					V[(opcode & 0x0F00) >> 8] += V[(opcode & 0x00F0) >> 4];
					break;
				case 0x0005: // SUB Vx, Vy
					V[0xF] = (V[(opcode & 0x0F00) >> 8] > V[(opcode & 0x00F0) >> 4]);
					V[(opcode & 0x0F00) >> 8] -= V[(opcode & 0x00F0) >> 4];
					break;
				case 0x0006: // SHR Vx {, Vy}
					V[0xF] = (V[(opcode & 0x0F00) >> 8] & 0x01);
					V[(opcode & 0x0F00) >> 8] /= 2;
					break;
				case 0x0007: // SUBN Vx, Vy
					V[0xF] = V[(opcode & 0x00F0) >> 4] > V[(opcode & 0x0F00) >> 8];
					V[(opcode & 0x00F0) >> 4] -= V[(opcode & 0x0F00) >> 8];
					break;
				case 0x000E: // SHL Vx{, Vy}
					V[0xF] = V[(opcode & 0x0F00) >> 8] & 0x80;
					V[(opcode & 0x0F00) >> 8] *= 2;
					break;
			}
			PC += 2;
		case 0xA000: // LD I, addr
			I = (opcode & 0x0FFF);
			PC += 2;
			break;
		case 0xB000: // JP V0, addr
			PC = (opcode & 0x0FFF) + V[0x00];
			break;
		case 0xC000: // RND Vx, byte
			V[(opcode & 0x0F00) >> 8] = (std::rand() % 256) & (opcode & 0x00FF);
			PC += 2;
			break;
		case 0xD000: // DRW Vx, Vy, nibble
			// ...





			

		
	}


}


int main() {
	std::srand(std::time(nullptr));
	if(import_program() == -1) return -1;

	SDL_INIT_VIDEO;
	SDL_Window* window = SDL_CreateWindow("chip-8", 640, 320, SDL_WINDOW_RESIZABLE);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr);

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
				if(sreen_buffer[y][x]) {

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
