#6502: 6502.c rom_reader.c
#	gcc -o 6502 6502.c

6502: main.c
	gcc -o nesplay main.c

clean:
	-rm 6502
	
all:
	make 6502