CC=gcc

nesplay: main.c romreader.c
	$(CC) -I. -g -O0 -o nesplay main.c romreader.c 

clean:
	-rm nesplay

all:
	make nesplay