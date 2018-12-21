#include <stdio.h>
#include "debug.h"
#include "romreader.h"

/*
 * Debugs a ROM to stdout
 */
void debug_rom(NESRom rom) {

    printf("\nROM Header debug: \n");
    printf("---------------------------------------------------------------------------\n");

    debug_byte_stream(rom.header.raw, HDR_BLOCK_SIZE, 1, 0);

    printf("PRG ROM size: %dKB (%d bytes in %d 16KB blocks)\n",
            rom.header.prg_rom_size / 1024,
            rom.header.prg_rom_size,
            rom.header.prg_rom_blocks);

    printf("CHR ROM size: %dKB (%d bytes in %d 8KB blocks)\n",
            rom.header.chr_rom_size / 1024,
            rom.header.chr_rom_size,
            rom.header.chr_rom_blocks);

    printf("MAPPER: 0x%02x, %d, %s\n",
            rom.header.mapper,
            rom.header.mapper,
            hex_to_bin(rom.header.mapper));

    printf("PRG RAM Size: %dKB (%d bytes)\n",
            rom.header.prg_ram_size / 1024,
            rom.header.prg_ram_size);

    printf("FLAGS 6 values:  0x%02x (%s)\n",
            rom.header.f6_raw,
            hex_to_bin(rom.header.f6_raw));

    debug_flags_6(rom.header);

    printf("FLAGS 7 values:  0x%02x (%s)\n",
            rom.header.f7_raw,
            hex_to_bin(rom.header.f7_raw));

    debug_flags_7(rom.header);

    printf("FLAGS 9 values:  0x%02x (%s)\n",
            rom.header.f9_raw,
            hex_to_bin(rom.header.f9_raw));

    printf("\tF9 TV SYSTEM: %s\n",
            rom.header.f9_std_pal ? "PAL" : "NTSC");

    printf("FLAGS 10 values: 0x%02x (%s)\n",
            rom.header.f10_raw,
            hex_to_bin(rom.header.f10_raw));

    debug_flags_10(rom.header);

    printf("\nPRG data debug: \n");
    printf("---------------------------------------------------------------------------\n");
    debug_byte_stream(rom.prg_rom, PRG_ROM_BLOCK_SIZE, 10, 0);

    printf("\nCHR data debug: \n");
    printf("---------------------------------------------------------------------------\n");
    debug_byte_stream(rom.chr_rom, CHR_ROM_BLOCK_SIZE, 10, 0);

}

/**
 * Debugs flags 6
 */
void debug_flags_6(NESHeader header) {

    if (header.f6_vertical_mirroring) {
        printf("\tMirroring: Horizontal (vertical arrangement) (CIRAM A10 = PPU A11)\n");
    } else {
        printf("\tMirroring: Vertical (horizontal arrangement) (CIRAM A10 = PPU A10)\n");
    }

    printf("\tPRG RAM BATTERY: %s.\n",
            header.f6_battery_prg_ram ? "Yes" : "No");

    printf("\tTrainer provided: %s.\n",
            header.f6_trainer_incuded
            ? "Yes: 512-byte trainer at $7000-$71FF" : "No");

    printf("\tMirroring control ignored: %s.\n",
            header.f6_ignore_mirror_control
            ? "Yes: the ROM should provide four-screen VRAM" : "No");

}

/**
 * Debugs flags 7
 */
void debug_flags_7(NESHeader header) {

    printf("\tUnisystem: %s.\n",
            header.f7_unisystem ? "Activated" : "Deactivated");

    printf("\tPlayChoice-10: %s.\n",
            header.f7_playchoice_10 ? "Activated" : "Deactivated");

    printf("\tNES 2.0: %s.\n",
            header.f7_nes20 ? "Yes" : "No");

}

/**
 * Debugs the non-standard flags 10.
 */
void debug_flags_10(NESHeader header) {

    char* tv_system;

    switch(header.f10_tv_system) {
    case TV_SYSTEM_NTSC:
        tv_system = "NTSC";
        break;
    case TV_SYSTEM_DUAL:
        tv_system = "DUAL";
        break;
    case TV_SYSTEM_PAL:
        tv_system = "PAL";
        break;
    }

    printf("\tF10 TV SYSTEM: %s.\n", tv_system);

    printf("\tPRG RAM present: %s.\n",
            header.f10_prg_ram ? "Yes" : "No");

    printf("\tBoard conflict: %s.\n",
            header.f10_board_conflict ? "Yes" : "No");

}

/**
 * Debugs a fixed-size byte stream in stdout.
 */
void debug_byte_stream(unsigned char* stream, size_t block_size, int num_blocks, size_t offset) {

    for (int block = 0 + offset; block < num_blocks + offset; block++) {
        printf("%02d: ", block);
        for (size_t i = 0; i < block_size; i++) {
            printf("%02x ", stream[i + (block_size - 1) * block]);
        }
        printf("\n");
    }

    printf("\n");

}

