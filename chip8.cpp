#include "chip8.h"
#include <algorithm>
#include <iostream>


const uint8_t fontset[] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0,
    0x20, 0x60, 0x20, 0x20, 0x70,
    0xF0, 0x10, 0xF0, 0x80, 0xF0,
    0xF0, 0x10, 0xF0, 0x10, 0xF0,
    0x90, 0x90, 0xF0, 0x10, 0x10,
    0xF0, 0x80, 0xF0, 0x10, 0xF0,
    0xF0, 0x80, 0xF0, 0x90, 0xF0,
    0xF0, 0x10, 0x20, 0x40, 0x40,
    0xF0, 0x90, 0xF0, 0x90, 0xF0,
    0xF0, 0x90, 0xF0, 0x10, 0xF0,
    0xF0, 0x90, 0xF0, 0x90, 0x90,
    0xE0, 0x90, 0xE0, 0x90, 0xE0,
    0xF0, 0x80, 0x80, 0x80, 0xF0,
    0xE0, 0x90, 0x90, 0x90, 0xE0,
    0xF0, 0x80, 0xF0, 0x80, 0xF0,
    0xF0, 0x80, 0xF0, 0x80, 0x80
};

Chip8::Chip8()
{
    std::copy(fontset, fontset + 80, mem);
    pc = 0x200;
    draw = false;
}

bool Chip8::loadROM(const char* filename)
{
    if (filename == NULL) {
        perror("Filename not provided");
        return false;
    }
    FILE* f = fopen(filename, "rb");
    if (f == NULL) {
        perror("Error while opening file");
        return false;
    }
    for (int i = 512; !feof(f) && i < 4096 - 512; i++) {
        fread(mem + i, sizeof(uint8_t), 1, f);
    }
    fclose(f);
    return true;
}

void Chip8::run_cycle()
{
    uint16_t opcode = getOpcode();
    runOpcode(opcode);
}


uint16_t Chip8::getOpcode()
{
    uint16_t opcode = (mem[pc] << 8 | mem[pc + 1]);
    pc += 2;
    return opcode;
}

void Chip8::runOpcode(uint16_t opcode)
{
    std::cout << "[INFO] RUNNING OPCODE " << std::hex << opcode << std::endl;
    uint16_t nnn = opcode & 0x0FFF;
    uint8_t n = opcode & 0x000F;
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t y = (opcode & 0x00F0) >> 4;
    uint8_t kk = opcode & 0x00FF;

    if (opcode == 0x00E0) {
        std::fill(gfx_mem, gfx_mem + (64 * 32), 0);
        draw = true;
    }
    else if (opcode == 0x00EE) {
        pc = stack[--sp];
    }
    else if ((opcode & 0xF000) == 0x1000) {
        pc = nnn;
    }
    else if ((opcode & 0xF000) == 0x2000) {
        stack[sp++] = pc;
        pc = nnn;
    }
    else if ((opcode & 0xF000) == 0x3000) {
        if (V[x] == kk) {
            pc += 2;
        }
    }
    else if ((opcode & 0xF000) == 0x4000) {
        if (V[x] != kk) {
            pc += 2;
        }
    }
    else if ((opcode & 0xF00F) == 0x5000) {
        if (V[x] == V[y]){
            pc +=2;
        }
    }
    else if ((opcode & 0xF000) == 0x6000) {
        V[x] = kk;
    }
    else if ((opcode & 0xF000) == 0x7000) {
        V[x] += kk;
    }
    else if ((opcode & 0xF00F) == 0x8000) {
        V[x] = V[y];
    }
    else if ((opcode & 0xF00F) == 0x8001) {
        V[x] |= V[y];
    }
    else if ((opcode & 0xF00F) == 0x8002) {
        V[x] &= V[y];
    }
    else if ((opcode & 0xF00F) == 0x8003) {
        V[x] ^= V[y];
    }
    else if ((opcode & 0xF00F) == 0x8004) {
        V[0xF] = (V[x] + V[y] > 0xFF);
        V[x] = (uint8_t)(V[x] + V[y]);
    }
    else if ((opcode & 0xF00F) == 0x8005) {
        V[0xF] = (V[x] > V[y]);
        V[x] -= V[y];
    }
    else if ((opcode & 0xF00F) == 0x8006) {
        V[0xF] = (V[x] & 0x1);
        V[x] /= 2;
    }
    else if ((opcode & 0xF00F) == 0x8007) {
        V[0xF] = (V[y] > V[x]);
        V[x] = V[y] - V[x];
    }
    else if ((opcode & 0xF00F) == 0x800E) {
        V[0xF] = (V[x] >> 7);
        V[x] /= 2;
    }
    else if ((opcode & 0xF00F) == 0x9000) {
        if (V[x] != V[y]) {
            pc += 2;
        }
    }
    else if ((opcode & 0xF000) == 0xA000) {
        I = nnn;
    }
    else if ((opcode & 0xF000) == 0xB000) {
        pc = nnn + V[0];
    }
    else if ((opcode & 0xF000) == 0xC000) {
        V[x] = (std::rand() % 256) & kk;
    }
    else if ((opcode & 0xF000) == 0xD000) {
        uint8_t sprite_byte;
        uint8_t x_val = V[x] % 64;
        uint8_t y_val = V[y] % 32;
        V[0xF] = 0;
        for (int i = 0; i < n; i++,y_val++) {
            if (y_val == 32) {
                break;
            }
            sprite_byte = mem[I+i];
            for (int j = 0; j < 8; j++) {
                if ((sprite_byte & (0x80 >> j)) !=0 ) {
                    if (gfx_mem[x_val + j + (y_val * 64)]) {
                        V[0xF] = 1;
                    }
                    gfx_mem[x_val + j + (y_val * 64)] ^= 1;
                }
                if (x_val + j == 64) {
                    break;
                }
            }
        }
        draw = true;
    }
    else {
        std::cout << "opcode " << opcode << " not implemented" << std::endl;
    }
}

