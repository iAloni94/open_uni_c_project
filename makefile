CC = gcc
CFLAGS = -pedantic -ansi -Wall -g

assembler: assembler.o input.o instruction.o to_binary.o utils.o write_files.o label.o directive.o first_pass.o second_pass.o
	${CC} ${CFLAGS} assembler.o -o assembler input.o instruction.o to_binary.o utils.o write_files.o label.o directive.o first_pass.o second_pass.o

assembler.o: assembler.c global.h directive.h
	${CC} ${CFLAGS} -c assembler.c

input.o: input.c global.h
	${CC} ${CFLAGS} -c input.c

instruction.o: instruction.c global.h
	${CC} ${CFLAGS} -c instruction.c

to_binary.o: to_binary.c instruction.h
	${CC} ${CFLAGS} -c to_binary.c

utils.o: utils.c global.h
	$(CC) $(CFLAGS) -c utils.c

write_files.o: write_files.c directive.h label.h utils.h
	${CC} ${CFLAGS} -c write_files.c

label.o: label.c global.h
	${CC} ${CFLAGS} -c label.c

directive.o: directive.c global.h
	${CC} ${CFLAGS} -c directive.c

first_pass.o: first_pass.c global.h
	${CC} ${CFLAGS} -c first_pass.c

second_pass.o: second_pass.c global.h
	${CC} ${CFLAGS} -c second_pass.c