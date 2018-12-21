/*
 * apu.h
 */

#ifndef APU_H_
#define APU_H_

// APU BUS is 32 bytes
#define APU_REG_SIZE 32

// Initializes the APU.
void apu_initialize();

// Reads a byte.
unsigned char apu_read_byte(unsigned short address);

// Writes a byte.
void apu_write_byte(unsigned short address, unsigned char byte);

#endif /* PPU_H_ */
