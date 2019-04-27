#include "chip8loadtests.h"

int main() {
    LoadTests loadTests;

    // Run all tests
    loadTests.runTests();

    
    std::cout << "All tests passed!" << std::endl;
}