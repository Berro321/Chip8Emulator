#include <stdio.h>
#include <iostream>

typedef unsigned short bit16;
typedef unsigned char bit8;

class Chip8Emulator {
public:
	Chip8Emulator() {}
	void initialize();
	void load(const char* file_name);
	void emulateCycle();

private:
	const size_t MAX_MEMORY_SIZE = 4096;
	bit16 opcode;  // Current operation (2 bytes)
	bit8 memory[4096]; // Memory
	bit8 V[16]; // V registers
	bit16 I;  // Index Register
	bit16 pc;  // program counter
	/* Memory Mapping
	0x000-0x1FF - Chip 8 interpreter (contains font set in emu)
	0x050-0x0A0 - Used for the built in 4x5 pixel font set (0-F)
	0x200-0xFFF - Program ROM and work RAM
	*/
	bit8 gfx[64 * 32];  // Screen is 64x32, only black and white (0 or 1)

	// Timers
	bit8 delay_timer;
	bit8 sound_timer;

	// Stack
	bit16 stack[16];
	bit16 sp;  // stack pointer

	bit8 key[16];  // Keyboard, represents values in hex from 0x0-0xF
};

void Chip8Emulator::initialize()
{
	// Initialize registers and memory once
	pc = 0x200;
	opcode = 0;
	I = 0;
	sp = 0;

	// Clear display
	// Clear stack
	// Clear registers
	// Clear memory

	// Load fontset

}

void Chip8Emulator::load(const char * file_name)
{
	FILE* gameFile;
	gameFile = fopen(file_name, "rb");
	int memory_ind = 0;  // First 512 are used for the interpreter
	while (!feof(gameFile))
	{
		if (memory_ind > 4096) {
			std::cerr << "Error, game file too big!" << std::endl;
		}
		// Copy game file data into memory
		memory[memory_ind + 512] =  getc(gameFile);
	}
}

// Emulate a single cycle of the CHIP8.
void Chip8Emulator::emulateCycle()
{
	// Fetch OpCode
	if (pc >= MAX_MEMORY_SIZE || pc + 1 >= MAX_MEMORY_SIZE)
	{
		std::cerr << "Error: Program counter has gone out of bounds!" << std::endl;
	}
	opcode = memory[pc] << 8 | memory[pc + 1];
	// Decode and execute Opcode
	switch (opcode & 0xF000)
	{
	case 0x0000:  // 0 cases
	{
		switch (opcode)
		{
		case 0x00E0: // Clear the screen
			std::cout << "Clearing screen" << std::endl;
			break;
		case 0x00EE:  // Return from subroutine
			pc = stack[sp];
			--sp;
			break;
		}
		break;
	}
	case 0x1000:  // 1NNN: Jump to location NNN
		pc = opcode & 0x0FFF;
		break;
	case 0x2000:  // 2NNN: Call subroutine at NNN
		++sp;
		stack[sp] = pc;
		pc = opcode & 0x0FFF;
		break;
	case 0x3000:  // 3xkk: Skip next instruction if Vx = kk
		bit8 reg = (opcode & 0x0F00) >> 16;
		bit16 val = opcode & 0x00FF;
		if (V[reg] == val)
		{
			pc += 4;
		}
		else
		{
			pc += 2;
		}
		break;
	case 0x4000:  // 4xkk: Skip next instruction if Vx != kk
		bit8 reg = (opcode & 0x0F00) >> 16;
		bit16 val = opcode & 0x00FF;
		if (V[reg] != val)
		{
			pc += 4;
		}
		else
		{
			pc += 2;
		}
		break;
	case 0x5000:  // 0x5xy0: Skip next instruction if Vx = Vy
		bit8 regx = (opcode & 0x0F00) >> 16;
		bit8 regy = (opcode & 0x00F0) >> 8;
		if (V[regx] == V[regy])
		{
			pc += 4;
		}
		else
		{
			pc += 2;
		}
		break;
	case 0xA000:  // ANNN: Set I to address NNN
		I = opcode & 0x0FFF;
		pc += 2;
		break;
	default:
		fprintf(stderr, "Error, wrong instruction: 0x%x", opcode);
	}
	// Update Timers
}

Chip8Emulator chip_emulator;

int main(int argc, char **argv)
{
	// Setup render system and register input callbacks (SDL)

	// Initialize the Chip8 system and laod game

	// Emulation loop
	for (;;)
	{
		return 0;
		// Take care of a draw flag -> only invoked by clear screen or draw sprite opcode
	}

}