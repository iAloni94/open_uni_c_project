CC = gcc
CFLAGS = -pedantic -ansi -Wall -g

assembler: assembler.o input.o r_type_func.o i_type_func.o j_type_func.o param_check.o num_base.o utils.o
	${CC} ${CFLAGS} assembler.o -o assembler input.o r_type_func.o i_type_func.o j_type_func.o  param_check.o num_base.o utils.o

assembler.o: assembler.c func.h param.h assembler.h utils.h
	${CC} ${CFLAGS} -c assembler.c

input.o: input.c assembler.h utils.h
	${CC} ${CFLAGS} -c input.c

param_check.o: param_check.c param.h assembler.h utils.h
	${CC} ${CFLAGS} -c param_check.c

r_type_func.o: r_type_func.c param.h assembler.h utils.h
	${CC} ${CFLAGS} -c r_type_func.c

i_type_func.o: i_type_func.c param.h assembler.h utils.h
	${CC} ${CFLAGS} -c i_type_func.c

j_type_func.o: j_type_func.c utils.h
	${CC} ${CFLAGS} -c j_type_func.c

num_base.o: num_base.c
	${CC} ${CFLAGS} -c num_base.c

utils.o: utils.c utils.h assembler.h
	$(CC) $(CFLAGS) -c utils.c