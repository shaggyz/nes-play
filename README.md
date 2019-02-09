# Playing with NES technology

Some experiments about 6502 CPU and NES technology. Just for fun. Binary garbage.

Tools used: `debian, vim, gcc, make, bash, i3-gaps, xterm, eclipse, git`

## Component list status

- ROM Cartridge reader emulator. *DONE*.
- MOS 6502 CPU emulator. **WIP**.
- RP2A03/APU Audio. *TODO*.
- PPU (have fun with this one!). *TODO*.

## Requirements

This project is plain C, implements the standard C library, in order to be portable. Successfully compiled in GNU/Linux, OSX and FreeBSD, other POSIX, potato and Unix-like systems should compile as well. Except MS Windows, I don't know/care if it compiles.

To compile you will need `gcc` and `make`.

And, of course, you will need `Super cow powers`, otherwise please proceed under your own risk.

## Compiling

```sh
make
```

## Running

Yo will need a NES rom called `super_mario_bros.nes` in `./roms` dir. ;-)

```sh
./nesplay roms/super_mario_bros.nes
```

## Evolution

### Started instruction set execution

Added debug for CPU instructions. Also, every bus message is displayed (RAM, PPU, APU, ROM, etc.).

```
CPU: Execution!
---------------------------------------------------------------------------
RAM: Initialized 2048 bytes (2KB).
PPU: Initialized.
APU: Initialized.
APU: Write at 0x4017 with 0x00.
APU: Write at 0x4015 with 0x00.
APU: Write at 0x4000 with 0x00.
APU: Write at 0x4001 with 0x00.
APU: Write at 0x4002 with 0x00.
APU: Write at 0x4003 with 0x00.
APU: Write at 0x4004 with 0x00.
APU: Write at 0x4005 with 0x00.
APU: Write at 0x4006 with 0x00.
APU: Write at 0x4007 with 0x00.
APU: Write at 0x4008 with 0x00.
APU: Write at 0x4009 with 0x00.
APU: Write at 0x400a with 0x00.
APU: Write at 0x400b with 0x00.
APU: Write at 0x400c with 0x00.
APU: Write at 0x400d with 0x00.
APU: Write at 0x400e with 0x00.
APU: Write at 0x400f with 0x00.
CPU: Preparing supported instructions.
CPU: Reading 32KB (32768 bytes) of PRG data.
ROM: read 0x78 from ROM address: 0x8000.
CPU: [+] Executed: SEI
ROM: read 0xd8 from ROM address: 0x8001.
CPU: [+] Executed: CLD
ROM: read 0xa9 from ROM address: 0x8002.
CPU ERROR: The instruction 0xa9 (169) is not implemented.
```

### ROM reader implemented

With a very basic debugger, example command `./nesplay roms/super_mario_bros.nes`:

```
Nico's NES emulator.
Loading ROM roms/super_mario_bros.nes...
Loaded 40KB (40976 bytes) of ROM data.
ROM file closed.

ROM Header debug: 
---------------------------------------------------------------------------
00: 4e 45 53 1a 02 01 01 00 10 a0 00 00 00 00 00 00 

PRG ROM size: 32KB (32768 bytes in 2 16KB blocks)
CHR ROM size: 8KB (8192 bytes in 1 8KB blocks)
MAPPER: 0x00, 0, 00000000
PRG RAM Size: 8KB (8192 bytes)
FLAGS 6 values:  0x01 (00000001)
	Mirroring: Horizontal (vertical arrangement) (CIRAM A10 = PPU A11)
	PRG RAM BATTERY: No.
	Trainer provided: No.
	Mirroring control ignored: No.
FLAGS 7 values:  0x00 (00000000)
	Unisystem: Deactivated.
	PlayChoice-10: Deactivated.
	NES 2.0: No.
FLAGS 9 values:  0x00 (00000000)
	F9 TV SYSTEM: NTSC
FLAGS 10 values: 0x00 (00000000)
	F10 TV SYSTEM: NTSC.
	PRG RAM present: Yes.
	Board conflict: No.

PRG data debug: 
---------------------------------------------------------------------------
00: 78 d8 a9 10 8d 00 20 a2 ff 9a ad 02 20 10 fb ad 
01: ad 02 20 10 fb a0 fe a2 05 bd d7 07 c9 0a b0 0c 
02: 0c ca 10 f6 ad ff 07 c9 a5 d0 02 a0 d6 20 cc 90 
03: 90 8d 11 40 8d 70 07 a9 a5 8d ff 07 8d a7 07 a9 
04: a9 0f 8d 15 40 a9 06 8d 01 20 20 20 82 20 19 8e 
05: 8e ee 74 07 ad 78 07 09 80 20 ed 8e 4c 57 80 01 
06: 01 a4 c8 ec 10 00 41 41 4c 34 3c 44 54 68 7c a8 
07: a8 bf de ef 03 8c 8c 8c 8d 03 03 03 8d 8d 8d 8d 
08: 8d 8d 8d 8d 8d 8d 8d 8d 00 40 ad 78 07 29 7f 8d 
09: 8d 78 07 29 7e 8d 00 20 ad 79 07 29 e6 ac 74 07 


CHR data debug: 
---------------------------------------------------------------------------
00: 03 0f 1f 1f 1c 24 26 66 
01: 66 00 00 00 00 1f 3f 3f 
02: 3f 7f e0 c0 80 fc 80 c0 
03: c0 00 20 00 20 60 00 f0 
04: f0 fc fe fe 60 70 18 07 
05: 07 0f 1f 3f 7f 7f 7f 1f 
06: 1f 07 00 1e 3f 7f fc 7c 
07: 7c 00 00 e0 f0 f8 f8 fc 
08: fc fc f8 c0 c2 67 2f 37 
09: 37 7f 7f ff ff 07 07 0f 

```

## Links

### NES

- iNes file format: https://wiki.nesdev.com/w/index.php/INES
- NES ROM header specs: http://nesdev.com/neshdr20.txt
- NES System: http://fms.komkon.org/EMUL8/NES.html
- ROM mappers: http://tuxnes.sourceforge.net/nesmapper.txt
- Amazing general document about NES console: http://nesdev.com/NESDoc.pdf

### MOS 6502

- OpCodes table: http://visual6502.org/wiki/index.php?title=6502_all_256_Opcodes
- CPU OpCodes: https://www.masswerk.at/6502/6502_instruction_set.html
- Hardware manual: http://archive.6502.org/books/mcs6500_family_hardware_manual.pdf
- Programming Manual: http://archive.6502.org/datasheets/synertek_programming_manual.pdf
- Arch.: http://nesdev.com/6502_cpu.txt
- 6502 ASM interpreter online: http://skilldrick.github.io/easy6502/
- Memory addressing modes: http://www.emulator101.com/6502-addressing-modes.html
- Memory map: https://en.wikibooks.org/wiki/NES_Programming#NES_2A03_CPU_memory_map
- CPU Power up: https://wiki.nesdev.com/w/index.php/CPU_power_up_state
- CPU Freq. thread on Reddit: https://www.reddit.com/r/EmuDev/comments/8cuvff/cpu_frequencycycles/


