#include <iostream>
#include <assert.h>
#include "../chip8emulator.h"

class LoadTests
{
public:
    LoadTests();
    void runTests();
private:
    void checkValuesDefault();
    void checkDataLoaded();
    Chip8Emulator emulator;
};