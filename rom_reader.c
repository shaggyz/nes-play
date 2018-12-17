#include "stdio.h"
#include "string.h"

#define ROM_LOADED 0
#define ROM_FILE_ERROR 1;

// ROMS dir.
static const char ROM_DIR[] = "./roms/";

// Default ROM files extension.
static const char ROM_EXT[] = ".nes";

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

	// Checking hex dump with: xxd roms/super_mario_bros.nes | head
    printf("----------------------------------------------\n");

	char* line = NULL;
	size_t len = 0;
	ssize_t read;

    while ((read = getline(&line, &len, rom_file)) != -1) {

		for (int i = 0; i < strlen(line); i++){
			unsigned char code = line[i];
			if (code < 16) {
				printf("0%x ", code);
			} else {
				printf("%x ", code);
			}
		}

		printf("\n------\n");

    }

    printf("\n----------------------------------------------\n");

    if (rom_file != NULL) {
        printf("Closing ROM file\n");
        fclose(rom_file);
    }

    return ROM_LOADED;
}


