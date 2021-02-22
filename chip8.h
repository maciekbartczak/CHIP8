#pragma once
#include <cstdint>

class Chip8 {
public:
	Chip8();
	bool loadROM(const char* filename);
	void run_cycle();
	void decrement_timers();
	bool draw;
	uint8_t gfx_mem[64 * 32]{};
	bool keypad[16]{};
private:
	uint8_t mem[4096]{};
	uint8_t V[16]{};
	uint16_t I{};
	uint16_t stack[16]{};
	uint8_t sp{};
	uint16_t pc;
	uint16_t getOpcode();
	void runOpcode(uint16_t opcode);
	uint8_t timer_delay{};
	uint8_t timer_sound{};
	

};