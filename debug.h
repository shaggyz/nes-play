/*
 * debug.h
 *
 * Dirty module to dump binary data on stdout.
 *
 */

#ifndef DEBUG_H_
#define DEBUG_H_

#include "romreader.h"

// How many 16-byte banks dump to stdout.
#define DEBUG_PRG_BANKS 20

/*
 * Debugs a ROM to stdout
 */
void debug_rom(NESRom rom);

/*
 * Debugs the flags 6 values.
 */
void debug_flags_6(NESHeader header);

/*
 * Debugs the flags 7 values.
 */
void debug_flags_7(NESHeader header);

/*
 * Debugs the flags 10 values.
 */
void debug_flags_10(NESHeader header);

/**
 * Debugs a fixed size byte stream on stdout.
 */
void debug_byte_stream(unsigned char* stream, size_t block_size, int amount, size_t offset);

#endif /* DEBUG_H_ */
