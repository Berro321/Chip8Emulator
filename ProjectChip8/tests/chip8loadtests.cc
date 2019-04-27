#include "chip8loadtests.h"

LoadTests::LoadTests()
{
    emulator.initialize();
}

void LoadTests::runTests()
{
    checkValuesDefault();
    checkDataLoaded();
}

void LoadTests::checkValuesDefault()
{
    // After being initialized, check that everything is initialized
    // to correct values.
    emulator.initialize();

    // Memory should be empty
    const bit8* memory = emulator.getmemory();
    for (int i = 0; i < 4096; ++i)
    {
        assert(memory[i] == 0x00);
    }

    // Stack should be empty
    const bit16* stack = emulator.getstack();
    for (int i = 0; i < 16; ++i)
    {
        assert(stack[i] == 0x00);
    }

    // Registers should be empty
    const bit8* vregister = emulator.getvregisters();
    for (int i = 0; i < 16; ++i)
    {
        assert(vregister[i] == 0x00);
    }

    assert(emulator.getprogramcounter() == 0x200);  // pc should start at 512

    // All other values should be 0
    assert(emulator.getstackpointer() == 0);
    assert(emulator.getindexreg() == 0);
}

void LoadTests::checkDataLoaded()
{
    emulator.initialize();
    // Check the proper program is loaded
    const char* filePath = "roms/games/Paddles.ch8";
    FILE* game = nullptr;
    emulator.load(filePath);

    if(fopen_s(&game, filePath, "rb"))
    {
        std::cerr << "Error while loading game file." << std::endl;
    }

    // Compare the data from the pc
    bit16 pc = emulator.getprogramcounter();
    const bit8* memory = emulator.getmemory();
    assert(pc == 512);
    while (!feof(game))
    {
        bit8 nchar = getc(game);
        assert(memory[pc] == nchar);
        ++pc;
    }
    fclose(game);
}