CC = gcc
CFLAGS = -pedantic -ansi -Wall -g

assembler: assembler.o input.o param.o to_binary.o utils.o write_files.o label.o directive.o
	${CC} ${CFLAGS} assembler.o -o assembler input.o param.o to_binary.o utils.o write_files.o label.o directive.o

assembler.o: assembler.c global.h directive.h
	${CC} ${CFLAGS} -c assembler.c

input.o: input.c global.h
	${CC} ${CFLAGS} -c input.c

param.o: param.c global.h
	${CC} ${CFLAGS} -c param.c

to_binary.o: to_binary.c instruction.h
	${CC} ${CFLAGS} -c to_binary.c

utils.o: utils.c global.h
	$(CC) $(CFLAGS) -c utils.c

write_files.o: write_files.c directive.h
	${CC} ${CFLAGS} -c write_files.c

label.o: label.c global.h
	${CC} ${CFLAGS} -c label.c

directive.o: directive.c directive.h
	${CC} ${CFLAGS} -c directive.c