/*
 * apu.c
 */

#include <stdio.h>
#include "apu.h"

// Internal APU stack. TODO: Implement.
unsigned char _register[APU_REG_SIZE];

// Initializes the internal RAM stack.
void apu_initialize() {
    for(unsigned short i; i < APU_REG_SIZE; i++) {
        _register[i] = 0x00;
    }
    printf("APU: Initialized.\n");
}

// --- MIRRORING ---

// PPU (8 bytes, with mirror 8KB)
// Mirrors of $2000-2007 (repeats every 8 bytes) $2008-$3FFF

// Reads a byte.
unsigned char apu_read_byte(unsigned short address) {
    // TODO: Handle mirroring.
    printf("APU: Read request for 0x%04x.\n", address);
    return 0x00;
}

// Writes a byte.
void apu_write_byte(unsigned short address, unsigned char byte) {
    // TODO: Handle mirroring.
    printf("APU: Write at 0x%04x with 0x%02x.\n", address, byte);
}
