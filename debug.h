/*
 * debug.h
 *
 * Dirty module to dump binary data on stdout.
 *
 */

#ifndef DEBUG_H_
#define DEBUG_H_

#include "romreader.h"

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

#endif /* DEBUG_H_ */
