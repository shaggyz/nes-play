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

// -----------------
// --- CPU Logic ---
// -----------------

/**
 * Initializes the CPU.
 */
void cpu_power_up() {

    printf("CPU: Start up!\n");
    // CPU Flags: NOEBDIZC
    //            00110100
    _set_flag(PF_INTERRUPT_DISABLE);
    _set_flag(PF_BREAK_CMD);
    _set_flag(PF_EXPANSION);

    WRITE_A(0x00);
    WRITE_X(0x00);
    WRITE_Y(0x00);

    WRITE_PS(0xfd);

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

    cpu_power_up();

    printf("CPU: Reading %dKB (%d bytes) of PRG data.\n",
            rom.header.prg_rom_size / 1024,
            rom.header.prg_rom_size);

    while (running) {

        unsigned char op_code = rom.prg_rom[PC];
        CpuInstruction operation = operations[op_code];

        // Check if the instruction is registered.
        if (operation.name[0] == '\0') {
            printf("CPU ERROR: The instruction 0x%02x (%d) is not implemented.", op_code, op_code);
            return false;
        }

        // Execute the instruction
        unsigned char parameters[2];
        parameters[0] = rom.prg_rom[PC + 0x01];
        parameters[1] = rom.prg_rom[PC + 0x02];

        // Test
        operation.op(parameters);
        printf("CPU: executed %s\n", operation.name);

        PC += (operation.param_c + 1);
        cpu_cycle += operation.cycles;
    }

    return true;

}
