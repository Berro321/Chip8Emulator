#include "chip8emulator.h"
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
    for (int i = 0; i < 16; ++i)
    {
        stack[i] = 0;
        V[i] = 0;
    }
	// Clear memory
    for (int i = 0; i < MAX_MEMORY_SIZE; ++i)
    {
        memory[i] = 0;
    }

	// Load fontset

}

void Chip8Emulator::load(const char * file_name)
{
	FILE* gameFile;
    errno_t fileopen_err = fopen_s(&gameFile, file_name, "rb"); 
	if (fileopen_err)
	{
		std::cerr << "Error loading in game file. Error Code: " << fileopen_err
            << std::endl;
	}
	int memory_ind = 0;  // First 512 are used for the interpreter
	while (!feof(gameFile))
	{
		if (memory_ind > MAX_MEMORY_SIZE) {
			std::cerr << "Error, game file too big!" << std::endl;
		}
		// Copy game file data into memory
		memory[memory_ind + 512] =  getc(gameFile);
        ++memory_ind;
	}
	fclose(gameFile);
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
	{
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
	}
	case 0x4000:  // 4xkk: Skip next instruction if Vx != kk
	{
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
	}
	case 0x5000:  // 0x5xy0: Skip next instruction if Vx = Vy
	{
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
	}
	case 0xA000:  // ANNN: Set I to address NNN
		I = opcode & 0x0FFF;
		pc += 2;
		break;
	default:
		fprintf(stderr, "Error, wrong instruction: 0x%x", opcode);
	}
	// Update Timers
}