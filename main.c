/*
 * Just for fun NES emulator.
 * Author: Nicolas Daniel Palumbo <n@xinax.net>
 */

#include <stdio.h>
#include <stdlib.h>
#include "romreader.h"

int main(int argc, char** argv) {

	if (argv[1] == NULL || !realpath(argv[1], NULL)) {
		fprintf(stderr,"ERROR: You need to specify a valid ROM file path.\n");
		return -1;
	}

	char* rom_path = argv[1];
	NESRom rom = load_rom(rom_path);

	if (rom.header.status != ROM_LOADED) {
		fprintf(stderr, "ERROR: The ROM %s cannot be loaded. ROM status: %d", rom_path, rom.header.status);
		return -1;
	}

	return 0;

}
