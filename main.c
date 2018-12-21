/*
 * Just for fun NES emulator.
 *
 * Author: Nicolas Daniel Palumbo <n@xinax.net>
 */

#include <stdio.h>
#include <stdlib.h>

#include "romreader.h"
#include "debug.h"
#include "6502.h"

int main(int argc, char** argv) {

    printf("Nico's NES emulator.\n");

    if (argv[1] == NULL || !realpath(argv[1], NULL)) {
        fprintf(stderr,"ERROR: You need to specify a valid ROM file path.\n");
        return -1;
    }

    NESRom rom = load_rom(argv[1]);

    // Some debug, for now.
    debug_rom(rom);

    // CPU test
    process_rom(rom);

    if (rom.header.status != ROM_LOADED) {
        fprintf(stderr, "ERROR: The ROM %s cannot be loaded. ROM status: %d", argv[1], rom.header.status);
        return -1;
    }

    return 0;

}
