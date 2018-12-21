/*
 * romreader.h
 */

#ifndef ROMREADER_H_
#define ROMREADER_H_

#include <stdbool.h>

/*
 * ROM reader response codes
 */
#define ROM_UNLOADED 0
#define ROM_LOADED 1
#define ROM_FILE_ERROR 2
#define ROM_HEADER_ERROR 3
#define ROM_HEADER_LOADED 4
#define ROM_NOT_SUPPORTED 5

/*
 * ROM header bytes (16, but 11-15 are not used)
 */
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

/*
 * TV systems
 */
#define TV_SYSTEM_NTSC 0
#define TV_SYSTEM_DUAL 1
#define TV_SYSTEM_PAL 2

/*
 * Internal buffers and block definitions
 */
#define PARSER_BLK_SIZE 16
#define PRG_ROM_BLOCK_SIZE 16
#define PRG_RAM_BLOCK_SIZE 8
#define CHR_ROM_BLOCK_SIZE 8
#define HDR_BLOCK_SIZE 16

/*
 * Nibble macros (4bit values H/L)
 */
#define HI_NIBBLE(b) (((b) >> 4) & 0x0F)
#define LO_NIBBLE(b) ((b) & 0x0F)

/*
 * NES Header struct: A high level struct with ROM metadata.
 */
typedef struct NES_HEADER {
    // --- FLAG 6 ---
    unsigned char f6_raw;
    bool f6_vertical_mirroring;
    bool f6_battery_prg_ram;
    bool f6_trainer_incuded;
    bool f6_ignore_mirror_control;
    // --- FLAG 7 ---
    unsigned char f7_raw;
    bool f7_unisystem;
    bool f7_playchoice_10;
    unsigned char f7_nes20;
    // --- FLAG 9 ---
    unsigned char f9_raw;
    bool f9_std_pal; 				// Standard NTSC/PAL flag. Not used.
    // --- FLAG 10 ---
    unsigned char f10_raw;
    unsigned char f10_tv_system; 	// relatively few emulators honor it
    bool f10_prg_ram; 				// PRG RAM ($6000-$7FFF) (0: present; 1: not present)
    bool f10_board_conflict;
    // --- General ---
    unsigned char status;
    unsigned char raw[16];
    unsigned char mapper;
    unsigned char prg_rom_blocks; 	// Number of 16 KB PRG blocks.
    size_t prg_rom_size; 			// Size in bytes of PRG ROM.
    unsigned char chr_rom_blocks; 	// Number of 8 KB CHR blocks.
    size_t chr_rom_size; 			// Size in bytes of CHR ROM.
    unsigned char prg_ram_blocks; 	// 8 KB block size: Value 0 infers 8 KB for compatibility.
    size_t prg_ram_size; 			// Size in bytes of PRG RAM.
} NESHeader;

/*
 * NES Rom struct: It contains the ROM code
 */
typedef struct NES_ROM {
    unsigned char* prg_rom;
    unsigned char* chr_rom;
    NESHeader header;
} NESRom;

/*
 * ROM header token.
 */
static const char HEADER_TOKEN[] = {0x4e, 0x45, 0x53, 0x1a};

/*
 * Loads a NES ROM FILE.
 */
NESRom load_rom(char* rom_path);

/*
 * Parses the ROM contents.
 */
NESRom parse_rom(FILE* rom_file);

/*
 * Parses the NES file header.
 */
NESHeader parse_header(unsigned char* raw_header);

/*
 * Creates a binary string representation of a byte.
 */
char* hex_to_bin(unsigned char byte);

#endif /* ROMREADER_H_ */
