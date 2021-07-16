CC = gcc
CFLAGS = -pedantic -ansi -Wall -g

assembler: assembler.o input.o r_type_func.o i_type_func.o j_type_func.o param_check.o
	${CC} ${CFLAGS} assembler.o -o assembler input.o r_type_func.o i_type_func.o j_type_func.o  param_check.o

assembler.o: assembler.c input.h func.h param.h
	${CC} ${CFLAGS} -c assembler.c

input.o: input.c input.h
	${CC} ${CFLAGS} -c input.c

param_check.o: param_check.c param.h
	${CC} ${CFLAGS} -c param_check.c

r_type_func.o: r_type_func.c
	${CC} ${CFLAGS} -c r_type_func.c

i_type_func.o: i_type_func.c
	${CC} ${CFLAGS} -c i_type_func.c

j_type_func.o: j_type_func.c
	${CC} ${CFLAGS} -c j_type_func.c
