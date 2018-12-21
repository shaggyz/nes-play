/*
 * 6502.c
 */

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "romreader.h"
#include "6502.h"

// ---------------------
// --- CPU Internals ---
// ---------------------

// CPU cycle counter. The integer limit is desired.
unsigned int cpu_cycle = 0x00;

// Simulation control.
bool running = false;

// CPU registers
unsigned char A;
unsigned char X;
unsigned char Y;

// Program counter
unsigned short PC = 0x00;

// Pointer stack
unsigned short PS = 0x00;

// Processor status flags
unsigned char P_FLAGS = 0x00;

bool _read_flag(unsigned char flag) {
    return P_FLAGS & (1 << flag);
}
void _set_flag(unsigned char flag) {
    P_FLAGS |= (1 << flag);
}
void _clear_flag(unsigned char flag) {
    P_FLAGS &= ~(1 << flag);
}

// -----------
// --- I/O ---
// -----------

NESRom _loaded_rom;

// --------------------
// --- CPU OP Codes ---
// --------------------

CpuInstruction operations[256];

// SEI
void OP_SEI(unsigned char params[2]) {
    _set_flag(PF_INTERRUPT_DISABLE);
}

/**
 * The operations register is populated with the implemented instructions
 */
void cpu_map_instructions() {

    CpuInstruction SEI;
    SEI.opcode = 0x78;
    SEI.cycles = 0x01;
    SEI.param_c = 0x00;
    SEI.mem_mode = MEM_A_IMPLIED;
	strcpy(SEI.name, "SEI");
	SEI.op = OP_SEI;
	operations[SEI.opcode] = SEI;

}

/**
 * Writes to data bus: IO device, APU, RAM, etc.
 */
bool cpu_bus_write(unsigned short address, unsigned char byte) {

    if (address >= 0x0000 && address <= 0x1fff) {
        // RAM (2KB, with mirror 8KB)
        // $0000-$07FF: 2K CPU RAM
        // MIRROR 1: $0800-$0FFF
        // MIRROR 1: $1000-$17FF
        // MIRROR 1: $1800-$1FFF
        // With mirroring: $0000-$1FFF (8KB in total, 8191 bytes)
        printf("RAM: Write requested at 0x%04x with 0x%02x.\n", address, byte);
    } else if (address >= 0x2000 && address <= 0x3FFF) {
        // PPU (8 bytes, with mirror 8KB)
        // Mirrors of $2000-2007 (repeats every 8 bytes) $2008-$3FFF
        printf("PPU: write requested at 0x%04x with 0x%02x.\n", address, byte);
    } else if (address >= 0x4000 && address <= 0x401F) {
        // APU (32 bytes)
        printf("APU: write requested at 0x%04x with 0x%02x.\n", address, byte);
    } else if (address >= 0x8000 && address <= 0xFFFF) {
        // ROM (32KB)
        fprintf(stderr, "IO: ERROR: Trying to write 0x%02x to a ROM address: 0x%04x.\n");
        return false;
    } else {
        // Unimplemented memory region
        fprintf(stderr, "IO: ERROR: Missing write handler for address: 0x%04x. Trying to write: 0x%02x.\n");
        return false;
    }

    return true;
}

/**
 * Reads from data bus.
 */
unsigned char cpu_bus_read(unsigned short address) {

    unsigned char _value = 0x00;

    if (address >= 0x0000 && address <= 0x1fff) {
        // RAM (2KB, with mirror 8KB)
        // $0000-$07FF: 2K CPU RAM
        // MIRROR 1: $0800-$0FFF
        // MIRROR 1: $1000-$17FF
        // MIRROR 1: $1800-$1FFF
        // With mirroring: $0000-$1FFF (8KB in total, 8191 bytes)
        printf("RAM: read requested for 0x%04x.\n", address);
    } else if (address >= 0x2000 && address <= 0x3FFF) {
        // PPU (8 bytes, with mirror 8KB)
        // Mirrors of $2000-2007 (repeats every 8 bytes) $2008-$3FFF
        printf("PPU: read requested for 0x%04x.\n", address);
    } else if (address >= 0x4000 && address <= 0x401F) {
        // APU (32 bytes)
        printf("APU: read requested for 0x%04x.\n", address);
    } else if (address >= 0x8000 && address <= 0xFFFF) {
        // ROM (32KB)
        _value = _loaded_rom.prg_rom[address - 0x8000];
        printf("ROM: read 0x%02x from ROM address: 0x%04x.\n", _value, address);
    } else {
        // Unimplemented memory region
        fprintf(stderr, "IO: Missing read handler for address: 0x%04x.\n", address);
    }

    return _value;
}

// -----------------
// --- CPU Logic ---
// -----------------

/**
 * Initializes the CPU.
 */
void cpu_power_up() {

    printf("CPU: Execution!\n");
    printf("---------------------------------------------------------------------------\n");
    // CPU Flags: NOEBDIZC
    //            00110100
    _set_flag(PF_INTERRUPT_DISABLE);
    _set_flag(PF_BREAK_CMD);
    _set_flag(PF_EXPANSION);

    WRITE_A(0x00);
    WRITE_X(0x00);
    WRITE_Y(0x00);

    WRITE_PS(0xfd);
    // TODO: This is probably wrong.
    // TODO: This value should be taken from $FFFD (reset handler)
    WRITE_PC(0x8000);

    // Frame IRQ enabled.
    cpu_bus_write(0x4017, 0x00);
    // all sound channels disabled
    cpu_bus_write(0x4015, 0x00);

    // Disable APU
    for(unsigned short address = 0x4000; address <= 0x400F; address ++) {
        cpu_bus_write(address, 0x00);
    }

    printf("CPU: Preparing supported instructions.\n");
    cpu_map_instructions();

    running = true;

}

/**
 * Resets the CPU states.
 */
void cpu_reset() {

    printf("CPU: RESET NOT IMPLEMENTED!\n");

}

/**
 * Process a NES ROM.
 */
bool cpu_process_rom(NESRom rom) {

    _loaded_rom = rom;

    cpu_power_up();

    printf("CPU: Reading %dKB (%d bytes) of PRG data.\n",
            _loaded_rom.header.prg_rom_size / 1024,
            _loaded_rom.header.prg_rom_size);

    while (running) {

        unsigned char op_code = cpu_bus_read(PC);
        CpuInstruction operation = operations[op_code];

        // Check if the instruction is registered.
        if (operation.name[0] == '\0') {
            fprintf(stderr, "CPU ERROR: The instruction 0x%02x (%d) is not implemented.", op_code, op_code);
            return false;
        }

        // Execute the instruction
        unsigned char parameters[2];
        parameters[0] = _loaded_rom.prg_rom[PC + 0x01];
        parameters[1] = _loaded_rom.prg_rom[PC + 0x02];

        // Test
        operation.op(parameters);
        printf("CPU: executed %s\n", operation.name);

        PC += (operation.param_c + 1);
        cpu_cycle += operation.cycles;
    }

    return true;

}
