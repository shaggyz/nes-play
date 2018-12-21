/*
 * 6502.h
 */

#ifndef H_6502_H_
#define H_6502_H_

#include "6502.h"
#include "romreader.h"

typedef struct CPU_INSTRUCTION {
    unsigned char opcode;
    char name[3];
    unsigned char cycles;
    unsigned char param_c;
    unsigned char (*op)(unsigned char[2]);
} CpuInstruction;

bool process_rom(NESRom rom);

#endif /* H_6502_H_ */
