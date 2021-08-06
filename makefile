CC = gcc
CFLAGS = -pedantic -ansi -Wall -g

assembler: assembler.o input.o param.o to_binary.o utils.o write_files.o
	${CC} ${CFLAGS} assembler.o -o assembler input.o param.o to_binary.o utils.o write_files.o

assembler.o: assembler.c global.h
	${CC} ${CFLAGS} -c assembler.c

input.o: input.c global.h
	${CC} ${CFLAGS} -c input.c

param.o: param.c global.h
	${CC} ${CFLAGS} -c param.c

to_binary.o: to_binary.c instruction.h
	${CC} ${CFLAGS} -c to_binary.c

utils.o: utils.c assembler.h utils.h 
	$(CC) $(CFLAGS) -c utils.c

write_files.o: write_files.c
	${CC} ${CFLAGS} -c write_files.c