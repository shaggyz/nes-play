/*
 * ppu.h
 */

#ifndef PPU_H_
#define PPU_H_

// PPU BUS is 8 bytes
#define PPU_REG_SIZE 8

// Initializes the PPU.
void ppu_initialize();

// Reads a byte.
unsigned char ppu_read_byte(unsigned short address);

// Writes a byte.
void ppu_write_byte(unsigned short address, unsigned char byte);

#endif /* PPU_H_ */
