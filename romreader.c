/*
 * romreader.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "romreader.h"

/*
 * Process the NES ROM file header, also loads the ROM header in the NESHeader struct
 */
NESHeader parse_header(unsigned char* raw_header) {

	NESHeader header;
	header.status = ROM_UNLOADED;

    // Check the file token
    bool token_checked = HEADER_TOKEN[HDR_TOKEN_0] == raw_header[HDR_TOKEN_0] &&
        HEADER_TOKEN[HDR_TOKEN_1] == raw_header[HDR_TOKEN_1] &&
        HEADER_TOKEN[HDR_TOKEN_2] == raw_header[HDR_TOKEN_2] &&
        HEADER_TOKEN[HDR_TOKEN_3] == raw_header[HDR_TOKEN_3];

    if (!token_checked) {
    	return header;
    }

    // Header values
    strcpy(header.raw, raw_header);
    header.status = ROM_HEADER_LOADED;
    header.prg_rom_blocks = raw_header[HDR_PRG_ROM_SIZE];
    header.chr_rom_blocks = raw_header[HDR_CHR_ROM_SIZE];

    // FLAGS 6
    unsigned char flags_6 = raw_header[HDR_FLAGS_6];

    header.f6_vertical_mirroring = flags_6 & (1 << 0);
    header.f6_battery_prg_ram = flags_6 & (1 << 1);
    header.f6_trainer_incuded = flags_6 & (1 << 2);
    header.f6_ignore_mirror_control = flags_6 & (1 << 3);
    header.mapper = HI_NIBBLE(flags_6);

    // FLAGS 7
	unsigned char flags_7 = raw_header[HDR_FLAGS_7];
	header.f7_unisystem = flags_7 & (1 << 0);
	header.f7_playchoice_10 = flags_7 & (1 << 1);
	header.f7_nes20 = (flags_7 & (1 << 2) == false) && (flags_7 & (1 << 3) == true);
	// This is probably in the wrong order: We are creating the mapper number
	// from HI/LO nibbles. There is no documentation about the nibbles HI/LO order
	// in the final mapper byte.
	header.mapper = header.mapper & HI_NIBBLE(flags_7);

	// FLAGS 8
	header.prg_ram_blocks = raw_header[HDR_FLAGS_8];

	// FLAGS 9
	unsigned char flags_9 = raw_header[HDR_FLAGS_9];
	header.f9_std_pal = flags_9 & (1 << 0);

	// FLAGS 10
	unsigned char flags_10 = raw_header[HDR_FLAGS_10];
	header.f10_tv_system = (((1 << 1) - 1) & (flags_10 >> (2 - 1)));
	header.f10_prg_ram = flags_10 & (1 << 4);
	header.f10_board_conflict = flags_10 & (1 << 5);

	return header;

}

/*
 * Parses the ROM file contents.
 */
NESRom parse_rom(FILE* rom_file) {

	NESRom rom;

    unsigned char buffer[PARSER_BLK_SIZE] = {0};
    unsigned int block_c = 0;
    size_t rbtyes, rheader = 0;
    size_t i, rbytes_sz = sizeof buffer;

    // Read the entire file in 16-byte blocks.
    while ((rbtyes = fread(buffer, sizeof *buffer, rbytes_sz, rom_file)) == rbytes_sz) {

    	if (0 == block_c) {
    		rom.header = parse_header(buffer);
        	// Header error.
    		if (rom.header.status != ROM_HEADER_LOADED) {
    			printf("HEADER ERROR\n");
    			return rom;
    		}
    	}

    	printf("%02d: ", block_c);
		for (i = 0; i < rbytes_sz; i++) {
			printf("%02x ", buffer[i]);
		}
		putchar('\n');

    	block_c++;

    	// TODO: Remove limiter.
    	if (11 == block_c) {
    		break;
    	}
    }

    // A final iteration over rbytes can be added for
	// remaining bytes outside the buffer (< 16);
    rom.header.status = ROM_LOADED;

	return rom;

}

/*
 * Loads a NES ROM file inside a custom emulator structure.
 */
NESRom load_rom(char* rom_path) {

	printf("Loading ROM %s...\n", rom_path);

	FILE* rom_file;
	rom_file = fopen(rom_path, "rb");

	NESRom rom = parse_rom(rom_file);

    if (rom_file == NULL) {
        perror("Error opening the ROM file\n");
        rom.header.status = ROM_FILE_ERROR;
        return rom;
    }

    if (rom_file != NULL) {
        printf("Closing ROM file\n");
        fclose(rom_file);
    }

	return rom;

}


/**
 * Debugs a NES ROM on stdout.
 */
void debug_rom(NESRom rom) {
    // Some display
    /*printf("\n---------------------------------------------------------------------------\n");
    printf("PRG ROM size: %d (16KB blocks) %dKB in total.\n", prg_rom_blocks, prg_rom_size);
    printf("CHR ROM size: %d (8KB blocks) %dKB in total.\n", chr_rom_blocks, chr_rom_size);
    printf("FLAG BANK #6: 0x%02x (%s)", flag_6, hex_to_bin(flag_6));
    printf("\n---------------------------------------------------------------------------\n\n");*/
}

/**
 * Creates a binary string representation of a byte.
 **/
char* hex_to_bin(unsigned char byte) {

    char* ret = malloc(sizeof byte);
	unsigned char bit, index = 0;

    for (bit = 1 << 7; bit > 0; bit = bit / 2) {
        ret[index] = (byte & bit) ? '1' : '0';
        index++;
	}

    return ret;

}
