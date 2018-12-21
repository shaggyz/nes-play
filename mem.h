/*
 * mem.h
 */

#ifndef MEM_H_
#define MEM_H_

// Total ram 2KB
#define RAM_SIZE 2048

// Initializes the internal RAM stack.
void ram_initialize();

// Reads a byte.
unsigned char ram_read_byte(unsigned short address);

// Writes a byte.
void ram_write_byte(unsigned short address, unsigned char byte);

#endif /* MEM_H_ */
