#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <cstdint>

class Chip8 {
public:
	Chip8();
	bool loadROM(const char* filename);
	void run_cycle();
	bool draw;
	uint8_t gfx_mem[64 * 32]{};
private:
	uint8_t mem[4096]{};
	uint8_t V[16]{};
	uint16_t I{};
	uint16_t stack[16]{};
	uint8_t sp{};
	uint16_t pc;
	void runCycle();
	uint16_t getOpcode();
	void runOpcode(uint16_t opcode);

};