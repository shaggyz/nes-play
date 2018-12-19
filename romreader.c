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

unsigned char* read_rom_bytes(unsigned char* rom, size_t r_bytes, size_t pos) {

	unsigned char* buffer = malloc(r_bytes + 1);
	size_t rom_size = strlen(rom);

	if (rom_size < (r_bytes - pos)) {
		fprintf(stderr, "Cannot read %d bytes. ROM length is: %d", r_bytes, rom_size);
		return NULL;
	}

	return buffer;
}

/*
 * Parses the ROM file contents.
 */
NESRom parse_rom(FILE* rom_file) {

	NESRom rom;

	// Read entire file size
	fseek(rom_file, 0, SEEK_END);
	size_t f_size = ftell(rom_file);
	fseek(rom_file, 0, SEEK_SET);

	// Read entire buffer
	unsigned char buffer[f_size + 1];
	fread(buffer, f_size, 1, rom_file);
	buffer[f_size] = 0;

	// Read header
	unsigned char header[HDR_BLOCK_SIZE + 1];
	memcpy(header, &buffer, HDR_BLOCK_SIZE);
	header[16] = '\0';

	rom.header = parse_header(header);
	size_t carry = HDR_BLOCK_SIZE;

	// Header error.
	if (rom.header.status != ROM_HEADER_LOADED) {
		return rom;
	}

	// Trainer
	if (rom.header.f6_trainer_incuded) {
		// 512KB trainer
		carry += 512;
		printf("ERROR: Trainer not implemented!!!");
		return rom;
	}

	// Read PRG ROM bank.
	size_t prg_rom_size = (PRG_ROM_BLOCK_SIZE * 1024) * rom.header.prg_rom_blocks;
	unsigned char prg_rom[prg_rom_size + 1];
	memcpy(prg_rom, &buffer[carry], prg_rom_size);
	prg_rom[prg_rom_size] = '\0';

	rom.prg_rom = prg_rom;
	carry += prg_rom_size;

	// Read CHR ROM bank
	size_t chr_rom_size = (CHR_ROM_BLOCK_SIZE * 1024) * rom.header.chr_rom_blocks;
	unsigned char chr_rom[chr_rom_size + 1];
	memcpy(chr_rom, &buffer[carry], chr_rom_size);
	chr_rom[chr_rom_size] = '\0';

	rom.chr_rom = chr_rom;
	carry += chr_rom_size;

	// Final checks
	if (carry != f_size) {
		fprintf(stderr, "Extra/Missing data for this ROM\n");
		fprintf(stderr, "ROM size: %d bytes.", f_size);
		fprintf(stderr, "Bytes read: %d bytes.", carry);
		rom.header.status = ROM_NOT_SUPPORTED;
		return rom;
	}

	printf("Loaded %dKB (%d bytes) of ROM data.\n", carry / 1024, carry);

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
        fclose(rom_file);
        printf("ROM file closed.\n");
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
