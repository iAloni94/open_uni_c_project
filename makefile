CC = gcc
CFLAGS = -pedantic -ansi -Wall -g

assembler: assembler.o input.o
	${CC} ${CFLAGS} assembler.o -o assembler input.o

assembler.o: assembler.c input.h
	${CC} ${CFLAGS} -c assembler.c

input.o: input.c input.h
	${CC} ${CFLAGS} -c input.c