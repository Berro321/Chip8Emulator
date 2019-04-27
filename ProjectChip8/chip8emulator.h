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

	// Accessor methods for debugging
	const bit8 * getvregisters() const { return V;}
	const bit8 * getmemory() const { return memory; }
	const bit16 * getstack() const { return stack; }
	const bit16 getstackpointer() const { return sp; }
	const bit16 getindexreg() const { return I; }
	const bit16 getprogramcounter() const { return pc; }

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