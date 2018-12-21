/*
 * mem.c
 */

#include "mem.h"
#include "stdio.h"

// Internal RAM stack
unsigned char _stack[RAM_SIZE];

// Initializes the internal RAM stack.
void ram_initialize() {
    for(unsigned short i; i < RAM_SIZE; i++) {
        _stack[i] = 0x00;
    }
    printf("RAM: Initialized %d bytes (%dKB).\n",
            RAM_SIZE,
            RAM_SIZE / 1024);
}

// --- MIRRORING ---

// $0000-$07FF: 2K CPU RAM
// MIRROR 1: $0800-$0FFF
// MIRROR 1: $1000-$17FF
// MIRROR 1: $1800-$1FFF
// With mirroring: $0000-$1FFF (8KB in total, 8191 bytes)

// Reads a byte.
unsigned char ram_read_byte(unsigned short address) {
    // TODO: Handle mirroring.
    printf("RAM: Read 0x%02x from 0x%04x.\n", _stack[address], address);
    return _stack[address];
}

// Writes a byte.
void ram_write_byte(unsigned short address, unsigned char byte) {
    // TODO: Handle mirroring.
    printf("RAM: Write at 0x%04x with 0x%02x.\n", address, byte);
    _stack[address] = byte;
}
