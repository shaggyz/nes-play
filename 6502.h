/*
 * 6502.h
 */

#ifndef H_6502_H_
#define H_6502_H_

#include "6502.h"
#include "romreader.h"

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

// Stack pointer
unsigned short SP = 0x00;

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

NESRom _bus_rom;

/**
 * Defines a CPU instruction struct.
 */
typedef struct CPU_INSTRUCTION {
    // HEX OpCode.
    unsigned char opcode;
    // CPU Cycles used by this instruction.
    unsigned char cycles;
    // Parameter counter (bytes)
    unsigned char param_c;
    // Memory addressing mode.
    char mem_mode;
    // Operation function pointer.
    void (*op)(unsigned char[2]);
    // Operation ASM name.
    char name[3];
} CpuInstruction;

#define DATA_BUS_SIZE 8

// Flags
#define PF_CARRY 0x00
#define PF_ZERO 0x01
#define PF_INTERRUPT_DISABLE 0x02
#define PF_DECIMAL_MODE 0x03
#define PF_BREAK_CMD 0x04 // No effect, used by stack copy.
#define PF_EXPANSION 0x05 // No effect, used by stack copy.
#define PF_OVERFLOW 0x06
#define PF_NEGATIVE 0x07

// -------------------------------
// --- Memory addressing modes ---
// -------------------------------

// No real memory access, used by operations to manage flags. It uses 3 CPU cycles. Example: CLC
#define MEM_A_IMPLIED 0x00

// Expecting 1 byte as parameter, which can be an immediate value or a relative memory address.
// 3 CPU Cycles. Example: ORA #$B2 (immediate constant value B2) or ORA $DF (relative memory address DF)
#define MEM_A_IMMEDIATE 0x01

// Absolute addressing, expects a 16 bit little-endian WORD with the memory address. 5 CPU Cycles.
// Example: JMP $4032 (in little-endian: 4C 32 40)
#define MEM_A_ABSOLUTE 0x02

// It uses a 16 bit little-endian WORD as parameter to read a memory address that contains another memory address.
// The JMP instruction is the only instruction that uses this addressing mode. The set the PC to the address stored at that address.
// Example: Memory: 1000 52 3a 04 d3 93 00 3f 93 84 Instruction: JMP ($1000)
// When this instruction is executed, the PC will be set to $3a52, which is the address stored at address $1000.
// CPU cycles?
#define MEM_A_INDIRECT 0x03

// Zero page: Access the first 256 bytes of the CPU's memory map (0x00-0xff)
// 4 CPU cycles. Example: LDA $35 (will put the value stored in memory location $35 into A)
#define MEM_A_ZEROPAGE 0x04

// Relative: only used for branch operations. The byte after the opcode is the branch offset.
// If the branch is taken, the new address will the the current PC plus the offset.
// The offset is a signed byte, so the offset only can be -127/+128. CPU Cycles: 4.
#define MEM_A_RELATIVE 0x05

// Absolute indexed: Adds an index value (for example register Y) to an absolute address.
// For example: STA $1000,Y (If Y=09 this instruction will store the content of A at address $1009)
// 6 CPU cycles.
#define MEM_A_IDX_ABSOLUTE 0x06

// Indexed zero page: Like absolute indexed, but the target address is limited to the first 0xFF bytes.
// Example: LDA $C0,X, and X is $60, then the target address will be $20. $C0+$60 = $120,
// but the carry is discarded in the calculation of the target address. 5 CPU Cycles.
#define MEM_A_IDX_ZEROPAGE 0x07

// Indexed indirect: This mode is only used with the X register. Consider a situation where the instruction is
// LDA ($20,X), X contains $04, and memory at $24 contains 0024: 74 20, First, X is added to $20 to get $24.
// The target address will be fetched from $24 resulting in a target address of $2074.
// Register A will be loaded with the contents of memory at $2074. 7 CPU Cycles.
#define MEM_A_IDX_INDIRECT 0x08

// This mode is only used with the Y register. It differs in the order that Y is applied to the indirectly
// fetched address. An example instruction that uses indirect index addressing is LDA ($86),Y .
// To calculate the target address, the CPU will first fetch the address stored at zero page location $86.
// That address will be added to register Y to get the final target address. 7 CPU Cycles.
// Indirect Indexed instructions are 2 bytes - the second byte is the zero-page address - $20 in the example.
// (So the fetched address has to be stored in the zero page.)
#define MEM_A_INDIRECT_IDX 0x09

// Some macros (we'll use this as logger decorators)
#define WRITE_A(v) (A = (unsigned char)v)
#define WRITE_X(v) (X = (unsigned char)v)
#define WRITE_Y(v) (Y = (unsigned char)v)

#define READ_A(v) (return (unsigned char)A)
#define READ_X(v) (return (unsigned char)X)
#define READ_Y(v) (return (unsigned char)Y)

#define READ_PC() (return PC)
#define READ_SP() (return SP)

#define WRITE_PC(v) (PC=v)
#define WRITE_SP(v) (SP=v)

// CPU Interface

/**
 * Power up the CPU. Sets the default register values.
 */
void cpu_power_up();

/**
 * Resets the CPU and registers to default status.
 * Some registers are unchanged after reset.
 */
void cpu_reset();

/**
 * Process a NES ROM. If the CPU is not running, cpu_power_up() will be called.
 * Otherwise current execution will be reset with cpu_reset()
 */
bool cpu_process_rom(NESRom rom);

/**
 * Stops the CPU execution.
 */
void cpu_stop();

#endif /* H_6502_H_ */
