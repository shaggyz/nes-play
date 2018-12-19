CC=gcc
CFLAGS=-I.

nesplay: main.c romreader.c
	$(CC) -o nesplay main.c romreader.c

clean:
	-rm nesplay

all:
	make nesplay