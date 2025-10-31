#include <iostream>
#include <stack>
#include <SDL3/SDL.h>
#include <cmath>
#include <fstream>
#include <cstdlib>
#include <ctime> 

std::string program_file = "C:\\Chip-8 pragrams\\Space Invaders [David Winter].ch8";
uint8_t memory[4096] = {0};
uint8_t V[16]; // registers v0 throught vF
uint16_t I{0}; // memory addresses register
uint8_t DT{0}; // delay timer register
uint8_t ST{0}; // sound timer register
uint16_t PC{0}; // program counter
bool key_pressed[16];
std::stack<uint16_t> stack; // stores the address that the PC will be set to when finishing a subroutine
bool screen_buffer[64][32] = {0}; // the screen state
bool running{true};
SDL_Event event;

void load_font() {

	memory[0x000] = 0xF0; // 0
	memory[0x001] = 0x90;
	memory[0x002] = 0x90;
	memory[0x003] = 0x90;
	memory[0x004] = 0xF0;

	memory[0x005] = 0x20; // 1
	memory[0x006] = 0x60;
	memory[0x007] = 0x20;
	memory[0x008] = 0x20;
	memory[0x009] = 0x70;

	memory[0x00A] = 0xF0; // 2
	memory[0x00B] = 0x10;
	memory[0x00C] = 0xF0;
	memory[0x00D] = 0x80;
	memory[0x00E] = 0xF0;

	memory[0x00F] = 0xF0; // 3
	memory[0x010] = 0x10;
	memory[0x011] = 0xF0;
	memory[0x012] = 0x10;
	memory[0x013] = 0xF0;

	memory[0x014] = 0x90; // 4
	memory[0x015] = 0x90;
	memory[0x016] = 0xF0;
	memory[0x017] = 0x10;
	memory[0x018] = 0x10;

	memory[0x019] = 0xF0; // 5
	memory[0x01A] = 0x80;
	memory[0x01B] = 0xF0;
	memory[0x01C] = 0x10;
	memory[0x01D] = 0xF0;

	memory[0x01E] = 0xF0; // 6
	memory[0x01F] = 0x80;
	memory[0x020] = 0xF0;
	memory[0x021] = 0x90;
	memory[0x022] = 0xF0;

	memory[0x023] = 0xF0; // 7
	memory[0x024] = 0x10;
	memory[0x025] = 0x20;
	memory[0x026] = 0x40;
	memory[0x027] = 0x40;

	memory[0x028] = 0xF0; // 8
	memory[0x029] = 0x90;
	memory[0x02A] = 0xF0;
	memory[0x02B] = 0x90;
	memory[0x02C] = 0xF0;

	memory[0x02D] = 0xF0; // 9
	memory[0x02E] = 0x90;
	memory[0x02F] = 0xF0;
	memory[0x030] = 0x10;
	memory[0x031] = 0xF0;

	memory[0x032] = 0xF0; // A
	memory[0x033] = 0x90;
	memory[0x034] = 0xF0;
	memory[0x035] = 0x90;
	memory[0x036] = 0x90;

	memory[0x037] = 0xE0; // B
	memory[0x038] = 0x90;
	memory[0x039] = 0xE0;
	memory[0x03A] = 0x90;
	memory[0x03B] = 0xE0;

	memory[0x03C] = 0xF0; // C
	memory[0x03D] = 0x80;
	memory[0x03E] = 0x80;
	memory[0x03F] = 0x80;
	memory[0x040] = 0xF0;

	memory[0x041] = 0xE0; // D
	memory[0x042] = 0x90;
	memory[0x043] = 0x90;
	memory[0x044] = 0x90;
	memory[0x045] = 0xE0;

	memory[0x046] = 0xF0; // E
	memory[0x047] = 0x80;
	memory[0x048] = 0xF0;
	memory[0x049] = 0x80;
	memory[0x04A] = 0xF0;

	memory[0x04B] = 0xF0; // F
	memory[0x04C] = 0x80;
	memory[0x04D] = 0xF0;
	memory[0x04E] = 0x80;
	memory[0x04F] = 0x80;
}

int import_program() {

	load_font();
	std::ifstream program(program_file, std::ios::binary);
	if(!program.is_open()) {
		std::cerr << "could not open file!";
		return -1;
	}
	program.read(reinterpret_cast<char*>(memory + 0x200), 3584);
	PC = 0x200;
	return 0;
}

void sdl_event() {
	while(SDL_PollEvent(&event)) {
		if(event.type == SDL_EVENT_QUIT) {
			running = false;
		} else if(event.type == SDL_EVENT_KEY_DOWN) {

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
		} else if(event.type == SDL_EVENT_KEY_UP) {

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

void chip_8_cycle() {

	uint16_t opcode = (memory[PC] << 8) | (memory[PC + 1]);
	switch(opcode & 0xF000) {
		case 0x0000:
			switch(opcode & 0x00FF) {
				case 0x00E0: // CLS ✔
					std::memset(screen_buffer, 0, sizeof(screen_buffer));
					break;
				case 0x00EE: // RET
					if(!stack.empty()) {
						PC = stack.top();
						stack.pop();
					}
					break;
				default: // SYS addr (ignored)
					break;
			}
			PC += 2;
			break;
		case 0x1000: // JP addr
			PC = opcode & 0x0FFF;
			break;
		case 0x2000: // CALL addr
			stack.push(PC + 2);
			PC = opcode & 0x0FFF;
			break;
		case 0x3000: // SE Vx, byte
			if(V[(opcode & 0x0F00) >> 8] == (opcode & 0x00FF)) PC += 4;
			else PC += 2;
			break;
		case 0x4000: // SNE Vx, byte
			if(V[(opcode & 0x0F00) >> 8] != (opcode & 0x00FF)) PC += 4;
			else PC += 2;
			break;
		case 0x5000: // SE Vx, Vy
			if(V[(opcode & 0x0F00) >> 8] == V[(opcode & 0x00F0) >> 4]) PC += 4;
			else PC += 2;
			break;
		case 0x6000: // LD Vx, byte
			V[(opcode & 0x0F00) >> 8] = (opcode & 0x00FF);
			PC += 2;
			break;
		case 0x7000: // ADD Vx, byte
			V[(opcode & 0x0F00) >> 8] += (opcode & 0x00FF);
			PC += 2;
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
					V[(opcode & 0x0F00) >> 8] >>= 1;
					break;
				case 0x0007: // SUBN Vx, Vy
					V[0xF] = V[(opcode & 0x0F00) >> 4] > V[(opcode & 0x0F00) >> 8];
					V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4] - V[(opcode & 0x0F00) >> 8];
					break;
				case 0x000E: // SHL Vx{, Vy}
					V[0xF] = V[(opcode & 0x0F00) >> 8] & 0x80;
					V[(opcode & 0x0F00) >> 8] <<= 1;
					break;
			}
			PC += 2;
			break;
		case 0x9000: // SNE Vx, Vy
			if(V[(opcode & 0x0F00) >> 8] != V[(opcode & 0x00F0) >> 4]) PC += 4;
			else PC += 2;
			break;
		case 0xA000: // LD I, addr
			I = (opcode & 0x0FFF);
			PC += 2;
			break;
		case 0xB000: // JP V0, addr
			PC = (opcode & 0x0FFF) + V[0x0];
			break;
		case 0xC000: // RND Vx, byte
			V[(opcode & 0x0F00) >> 8] = (std::rand() % 256) & (opcode & 0x00FF);
			PC += 2;
			break;
		case 0xD000: { // DRW Vx, Vy, nibble
			V[0xF] = 0;
			int sprite_x{V[(opcode & 0x0F00) >> 8]};
			int sprite_y{V[(opcode & 0x00F0) >> 4]};
			int changed_pixel{0};
			for(int y = 0; y < (opcode & 0x000F); y++) {
				for(int x = 0; x < 8; x++) {
					int x_position{sprite_x + x};
					int y_position{sprite_y + y};
					bool pixel = (memory[I] & (0x80 >> x));
					x_position = (x_position > 64 ? x_position - 64 : x_position < 0 ? x_position + 64 : x_position);
					y_position = (y_position > 32 ? y_position - 32 : y_position < 0 ? y_position + 32 : y_position);
					if(screen_buffer[x_position][y_position] == pixel && pixel == 1) changed_pixel = 1;
					screen_buffer[x_position][y_position] ^= pixel;
				}
				I++;
			}
			V[0xF] = (changed_pixel);
			PC += 2;
			break;
		}  
		case 0xE000:
			switch(opcode & 0x00FF) {
				case 0x009E: // SKP Vx
					if(key_pressed[V[(opcode & 0x0F00) >> 8]]) PC += 4;
					else PC += 2;
					break;
				case 0x00A1: // SKNP Vx
					if(!key_pressed[V[(opcode & 0x0F00) >> 8]]) PC += 4;
					else PC += 2;
					break;
			}
			break;
		case 0xF000:
			switch(opcode & 0x00FF) {
				case 0x0007: // LD Vx, DT
					V[(opcode & 0x0F00) >> 8] = DT;
					PC += 2;
					break;
				case 0x000A: // LD Vx, K
					while(true) {
						auto it = std::find(std::begin(key_pressed), std::end(key_pressed), true);
						if(it != std::end(key_pressed)) {
							V[(opcode & 0x0F00) >> 8] = std::distance(std::begin(key_pressed), it);
							break;
						}
					}
					PC += 2;
					break;
				case 0x0015: // LD DT, Vx
					DT = V[(opcode & 0x0F00) >> 8];
					PC += 2;
					break;
				case 0x0018: // LD ST, Vx
					ST = V[(opcode & 0x0F00) >> 8];
					PC += 2;
					break;
				case 0x001E: // ADD I, Vx
					I += V[(opcode & 0x0F00) >> 8];
					PC += 2;
					break;
				case 0x0029: // LD F, Vx
					I = V[(opcode & 0x0F00) >> 8] * 5;
					PC += 2;
					break;
				case 0x0033: { // LD B, Vx
					uint8_t Vx = V[(opcode & 0x0F00) >> 8];
					memory[I] = Vx / 100;
					memory[I + 1] = (Vx / 10) % 10;
					memory[I + 2] = Vx % 10;
					PC += 2;
					break;
				}
				case 0x0055: // LD [I], Vx
					for(int V_register = 0; V_register <= ((opcode & 0x0F00) >> 8); V_register++) {
						memory[I + V_register] = V[V_register];
					}
					PC += 2;
					break;
				case 0x0065: // LD Vx, [I]
					for(int V_register = 0; V_register <= ((opcode & 0x0F00) >> 8); V_register++) {
						V[V_register] = memory[I + V_register];
					}
					PC += 2;
					break;
			}
			break;
	}
}

int main() {
	std::srand(std::time(nullptr));
	if(import_program() == -1) return -1;

	SDL_INIT_VIDEO;
	SDL_Window* window = SDL_CreateWindow("chip-8", 640, 320, SDL_WINDOW_RESIZABLE);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr);

	while(running) {
		sdl_event();

		chip_8_cycle();

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		for(int y = 0; y < 32; y++) {
			for(int x = 0; x < 64; x++) {
				if(screen_buffer[x][y]) {

					SDL_FRect pixel{x * 10.f, y * 10.f, 10.f, 10.f};
					SDL_RenderFillRect(renderer, &pixel);
				}
			}
		}
		SDL_RenderPresent(renderer);
		SDL_Delay(17);
	}
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}
