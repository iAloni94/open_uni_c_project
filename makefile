CC = gcc
CFLAGS = -pedantic -ansi -Wall -g

assembler: assembler.o input.o r_type_func.o i_type_func.o j_type_func.o param_check.o to_binary.o
	${CC} ${CFLAGS} assembler.o -o assembler input.o r_type_func.o i_type_func.o j_type_func.o  param_check.o to_binary.o

assembler.o: assembler.c input.h func.h param.h assembler.h
	${CC} ${CFLAGS} -c assembler.c

input.o: input.c input.h
	${CC} ${CFLAGS} -c input.c

param_check.o: param_check.c param.h assembler.h
	${CC} ${CFLAGS} -c param_check.c

r_type_func.o: r_type_func.c
	${CC} ${CFLAGS} -c r_type_func.c

i_type_func.o: i_type_func.c
	${CC} ${CFLAGS} -c i_type_func.c

j_type_func.o: j_type_func.c
	${CC} ${CFLAGS} -c j_type_func.c

to_binary.o: to_binary.c
	${CC} ${CFLAGS} -c to_binary.c