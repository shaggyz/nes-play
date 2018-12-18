#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

/**
 * ----------------------------------------------------------------------------
 * Simple NES cartridge ROM reader.
 * ----------------------------------------------------------------------------
 * Just for fun =). Author: Nicolas Daniel Palumbo <n@xinax.net>
 *
 * Contains utility functions to read binary data from iNes ROM files.
 * Nes 2.0 is not supported.
 **/

// Response codes
#define ROM_LOADED 0
#define ROM_READ 0
#define ROM_FILE_ERROR 1
#define ROM_HEADER_ERROR 1

// Rom header bytes
#define HDR_TOKEN_0 0
#define HDR_TOKEN_1 1
#define HDR_TOKEN_2 2
#define HDR_TOKEN_3 3
#define HDR_PRG_ROM_SIZE 4
#define HDR_CHR_ROM_SIZE 5
#define HDR_FLAGS_6 6
#define HDR_FLAGS_7 7
#define HDR_FLAGS_8 8
#define HDR_FLAGS_9 9
#define HDR_FLAGS_10 10

// Internal buffers and blocks
#define DEBUG_BUFSZ 16
#define PRG_ROM_BLOCK_SIZE 16
#define CHR_ROM_BLOCK_SIZE 8

#define HI_NIBBLE(b) (((b) >> 4) & 0x0F)
#define LO_NIBBLE(b) ((b) & 0x0F)

// ROM header token.
static const char HEADER_TOKEN[] = {0x4e, 0x45, 0x53, 0x1a};

// ROMS dir.
static const char ROM_DIR[] = "./roms/";

// Default ROM files extension.
static const char ROM_EXT[] = ".nes";

/**
 * NES Header struct: A high level struct with ROM metadata.
 */
typedef struct NES_HEADER {
	unsigned char raw[16];
	bool f6_horizontal_mirroring;
	bool f6_vertical_mirroring;
	bool f6_battery_prg_ram;
	bool f6_trainer_incuded;
	bool f6_ignore_mirror_control;
	unsigned char mapper;
} NESHeader;

/**
 * NES Rom struct: It contains the ROM code
 */
typedef struct NES_ROM {
	NESHeader header;
	unsigned char* prg_rom;
	unsigned char* chr_rom;
} NESRom;

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

/**
 * Some tests. Deprecated.
 */ 
void parse_flag_6(unsigned char flag6) {
    printf("Parsing flag 6 with value: %s\n", hex_to_bin(flag6));

	if (flag6 & (1<<0)) {
		printf("- horizontal (vertical arrangement) (CIRAM A10 = PPU A11)\n");	
	} else {
		printf("- vertical (horizontal arrangement) (CIRAM A10 = PPU A10)\n");	
	}

	if (flag6 & (1<<1)) {
		printf("- Cartridge contains battery-backed PRG RAM ($6000-7FFF) or other persistent memory\n");	
	} else {
		printf("- No battery backed PRG RAM detected.\n");	
	}

	if (flag6 & (1<<2)) {
		printf("512-byte trainer at $7000-$71FF (stored before PRG data)");	
	} else {
		printf("- No trainer detected.\n");	
	}

	if (flag6 & (1<<3)) {
		printf("Ignore mirroring control or above mirroring bit; instead provide four-screen VRAM\n");	
	} 

	// Extracting 4 bits from 4th position.
	// Lower nibble for mapper numbers: https://en.m.wikipedia.org/wiki/Nibble
	unsigned char mapper = (((1 << 4) - 1) & (flag6 >> (4 - 1)));

	if (mapper == 0x00) {
		printf("- Using NROM with first generic mapper.\n");
	} else {
		printf("Unknown mapper.\n");
	}
}

/**
 * Dumps the 16 byte ROM header.
 **/
bool dump_header(unsigned char header[]) {
    // Check the file token
    bool token_checked = HEADER_TOKEN[HDR_TOKEN_0] == header[HDR_TOKEN_0] &&
        HEADER_TOKEN[HDR_TOKEN_1] == header[HDR_TOKEN_1] &&
        HEADER_TOKEN[HDR_TOKEN_2] == header[HDR_TOKEN_2] &&
        HEADER_TOKEN[HDR_TOKEN_3] == header[HDR_TOKEN_3];

    if (token_checked == false) {
        return false;
    }

    // PRG ROM
    unsigned char prg_rom_blocks = header[HDR_PRG_ROM_SIZE];
    unsigned int prg_rom_size = prg_rom_blocks * PRG_ROM_BLOCK_SIZE;

    // CHR ROM
    unsigned char chr_rom_blocks = header[HDR_CHR_ROM_SIZE];
    unsigned int chr_rom_size = chr_rom_blocks * CHR_ROM_BLOCK_SIZE;

    // Flag bank #6
    unsigned char flag_6 = header[HDR_FLAGS_6];
    parse_flag_6(flag_6);

    // Some display
    printf("\n---------------------------------------------------------------------------\n");
    printf("PRG ROM size: %d (16KB blocks) %dKB in total.\n", prg_rom_blocks, prg_rom_size); 
    printf("CHR ROM size: %d (8KB blocks) %dKB in total.\n", chr_rom_blocks, chr_rom_size); 
    printf("FLAG BANK #6: 0x%02x (%s)", flag_6, hex_to_bin(flag_6));
    printf("\n---------------------------------------------------------------------------\n\n");

    for (char i = 0; i < DEBUG_BUFSZ; i++) {
        //printf("Header byte %02d with value: %02x (%c)\n", i, header[i], header[i]);
    }

    return true;
}

/**
 * Dumps the ROM data to STDOUT
 **/
int dump_rom_file(FILE* rom_file) {
    unsigned char buffer[DEBUG_BUFSZ] = {0};

    size_t rbtyes, rheader = 0;
    size_t i, rbytes_sz = sizeof buffer;
    unsigned int block_c = 0;

    while ((rbtyes = fread(buffer, sizeof *buffer, rbytes_sz, rom_file)) == rbytes_sz) {

        // Header dump
        if (0 == block_c && dump_header(buffer) == false)  {
            perror("Invalid NES ROM token.");
            return ROM_HEADER_ERROR;
        }
        
        printf("%02d: ", block_c);
        
        for (i = 0; i < rbytes_sz; i++) {
            printf("%02x ", buffer[i]);
        }
        
        putchar('\n');
        
        block_c++;
        return ROM_READ;
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

    unsigned char dump_status = dump_rom_file(rom_file);

    if (rom_file != NULL) {
        printf("Closing ROM file\n");
        fclose(rom_file);
    }

    if (dump_status != ROM_LOADED) {
        return dump_status;
    }

    return ROM_LOADED;
}


