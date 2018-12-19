CC=gcc

nesplay: main.c romreader.c debug.c 6502.c
	$(CC) -I. -g -O0 -o nesplay main.c romreader.c debug.c 6502.c 

clean:
	-rm nesplay

all:
	make nesplay