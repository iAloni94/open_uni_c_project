CC = gcc
CFLAGS = -pedantic -ansi -Wall -g

assembler: assembler.o input.o type_r_func.o type_i_func.o type_j_func.o param.o to_binary.o utils.o write_files.o
	${CC} ${CFLAGS} assembler.o -o assembler input.o type_r_func.o type_i_func.o type_j_func.o  param.o to_binary.o utils.o write_files.o

assembler.o: assembler.c functions.h
	${CC} ${CFLAGS} -c assembler.c

input.o: input.c assembler.h utils.h
	${CC} ${CFLAGS} -c input.c

param.o: param.c assembler.h utils.h 
	${CC} ${CFLAGS} -c param.c

type_r_func.o: type_r_func.c functions.h
	${CC} ${CFLAGS} -c type_r_func.c

type_i_func.o: type_i_func.c functions.h
	${CC} ${CFLAGS} -c type_i_func.c

type_j_func.o: type_j_func.c functions.h
	${CC} ${CFLAGS} -c type_j_func.c

to_binary.o: to_binary.c instruction.h
	${CC} ${CFLAGS} -c to_binary.c

utils.o: utils.c assembler.h utils.h 
	$(CC) $(CFLAGS) -c utils.c

write_files.o: write_files.c
	${CC} ${CFLAGS} -c write_files.c