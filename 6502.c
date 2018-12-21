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

#define DATA_BUS_SIZE 8

// CPU cycle counter. The integer limit is desired.
unsigned int cpu_cycle = 0;

// Accumulator register
unsigned char A;

// --------------------
// --- CPU OP Codes ---
// --------------------

#define SUM_OPCODE = 32

// ------------------------
// --- CPU Instructions ---
// ------------------------

CpuInstruction operations[256];

unsigned char OP_SUM(unsigned char params[2]) {

    unsigned char result = params[0] + params[1];
    printf("Processing instruction SUM with %d + %d, result: %d.\n", params[0], params[1], result);
    return result;

}

/**
 * The operations register is populated with the implemented instructions
 */
void map_instructions() {

    /*CpuInstruction SUM;
	SUM.cycles = 3;
	SUM.param_c = 2;
	strcpy(SUM.name, "SUM");
	SUM.op = OP_SUM;

	operations[0x78] = SUM;*/

}

/**
 * Process a NES ROM.
 */
bool process_rom(NESRom rom) {

    printf("CPU: Preparing supported instructions.\n");
    map_instructions();

    printf("CPU: Reading %dKB (%d bytes) of PRG data.\n",
            rom.header.prg_rom_size / 1024,
            rom.header.prg_rom_size);

    for(int index = 0; index < rom.header.prg_rom_size; index++) {

        unsigned char op_code = rom.prg_rom[index];
        CpuInstruction operation = operations[op_code];

        // Check if the instruction is registered.
        if (operation.name[0] == '\0') {
            printf("CPU ERROR: The instruction 0x%02x (%d) is not implemented.", op_code, op_code);
            return false;
        }

        // Execute the instruction
        unsigned char parameters[2];
        parameters[0] = rom.prg_rom[index + 0x01];
        parameters[1] = rom.prg_rom[index + 0x02];
        operation.op(parameters);

        cpu_cycle ++;
    }

    return true;

}
