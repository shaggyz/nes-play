CC=gcc

nesplay: main.c romreader.c debug.c 6502.c mem.c ppu.c apu.c
	$(CC) -I. -g -O0 -o nesplay main.c romreader.c debug.c 6502.c mem.c ppu.c apu.c 

clean:
	-rm nesplay

all:
	make nesplay