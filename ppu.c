/*
 * ppu.c
 */

#include <stdio.h>
#include "ppu.h"

// Internal PPU stack. TODO: Implement.
unsigned char _register[PPU_REG_SIZE];

// Initializes the internal RAM stack.
void ppu_initialize() {
    for(unsigned short i; i < PPU_REG_SIZE; i++) {
        _register[i] = 0x00;
    }
    printf("PPU: Initialized.\n");
}

// --- MIRRORING ---

// PPU (8 bytes, with mirror 8KB)
// Mirrors of $2000-2007 (repeats every 8 bytes) $2008-$3FFF

// Reads a byte.
unsigned char ppu_read_byte(unsigned short address) {
    // TODO: Handle mirroring.
    printf("PPU: Read request for 0x%04x.\n", address);
    return 0x00;
}

// Writes a byte.
void ppu_write_byte(unsigned short address, unsigned char byte) {
    // TODO: Handle mirroring.
    printf("PPU: Write at 0x%04x with 0x%02x.\n", address, byte);
}

