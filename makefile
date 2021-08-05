CC = gcc
CFLAGS = -pedantic -ansi -Wall -g

assembler: assembler.o input.o type_r_func.o type_i_func.o type_j_func.o param.o num_base.o utils.o write_files.o
	${CC} ${CFLAGS} assembler.o -o assembler input.o type_r_func.o type_i_func.o type_j_func.o  param.o num_base.o utils.o write_files.o

assembler.o: assembler.c assembler.h param.h  utils.h func.h
	${CC} ${CFLAGS} -c assembler.c

input.o: input.c assembler.h utils.h
	${CC} ${CFLAGS} -c input.c

param.o: param.c assembler.h param.h utils.h 
	${CC} ${CFLAGS} -c param.c

type_r_func.o: type_r_func.c assembler.h param.h  utils.h func.h
	${CC} ${CFLAGS} -c type_r_func.c

type_i_func.o: type_i_func.c assembler.h param.h  utils.h func.h
	${CC} ${CFLAGS} -c type_i_func.c

type_j_func.o: type_j_func.c assembler.h param.h  utils.h func.h
	${CC} ${CFLAGS} -c type_j_func.c

num_base.o: num_base.c param.h
	${CC} ${CFLAGS} -c num_base.c

utils.o: utils.c assembler.h utils.h 
	$(CC) $(CFLAGS) -c utils.c

write_files.o: write_files.c
	${CC} ${CFLAGS} -c write_files.c