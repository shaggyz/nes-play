#include <stdio.h>
#include "rom_reader.c"

// Processor registers
char X = 0;
char Y = 0;
char A = 0;

// Main static entry point
void main (char* argv, int argc) {
    printf("Nico's 6502 processor emulator.\n");
    read_rom_file("super_mario_bros");
}


