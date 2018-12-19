CC=gcc

nesplay: main.c romreader.c debug.c
	$(CC) -I. -g -O0 -o nesplay main.c romreader.c debug.c 

clean:
	-rm nesplay

all:
	make nesplay