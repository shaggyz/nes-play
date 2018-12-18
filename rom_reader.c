#include "stdio.h"
#include "string.h"

/**
 * ----------------------------------------------------------------------------
 * Simple NES cartridge ROM reader.
 * ----------------------------------------------------------------------------
 * Just for fun =). Author: Nicolas Daniel Palumbo <n@xinax.net>
 *
 * Contains utility functions to read binary data from iNes ROM files.
 * Nes 2.0 is not supported.
 **/

#define ROM_LOADED 0
#define ROM_FILE_ERROR 1
#define DEBUG_BUFSZ 16

// ROMS dir.
static const char ROM_DIR[] = "./roms/";

// Default ROM files extension.
static const char ROM_EXT[] = ".nes";

/**
 * Dumps the ROM data to STDOUT
 **/
void dump_rom_file(FILE* rom_file) {
    unsigned char buffer[DEBUG_BUFSZ] = {0};

    size_t rbtyes = 0;
    size_t i, rbytes_sz = sizeof buffer;

    while ((rbtyes = fread(buffer, sizeof *buffer, rbytes_sz, rom_file)) == rbytes_sz) {
        for (i = 0; i < rbytes_sz; i++) {
            printf("%02x ", buffer[i]);
        }
        putchar('\n');
    }
    // A final iteration over rbytes can be added for 
    // remaining bytes outside the buffer (< 16);
}

/**
 * Reads a NES rom file, from roms folder. 
 */
int read_rom_file(char* rom_name) {
    
    char rom_path[50] = "";
    strcat(rom_path, ROM_DIR);
    strcat(rom_path, rom_name);
    strcat(rom_path, ROM_EXT);

    printf("Reading rom: %s \n", rom_path);
    
    FILE* rom_file;
    rom_file = fopen(rom_path, "rb");

    if (rom_file == NULL) {
        perror("Error opening the ROM file\n");
        return ROM_FILE_ERROR;
    }

    dump_rom_file(rom_file);

    if (rom_file != NULL) {
        printf("Closing ROM file\n");
        fclose(rom_file);
    }

    return ROM_LOADED;
}


