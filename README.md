# open_uni_c_project
Assembler software for assembly language written in C.

Final project in the course “System ProgrammingLaboratory in C language - 20465”. Lecturer: Roy Rachmany This project was written by Ido Aloni and Yoav Brand

As part of the course “System ProgrammingLaboratory in C language”, we wrote this project that translates assembly language to “Machine Code” language. The main goal of this assembler is to create a file that contains “Machine Code” (output) from an assembly file (input).

Hardware: To emphasize the work and execution of a real computer, in this project our imaginary machine has: 32 registers, named $0, $1, $2…. $31. The size of each register is 32 bits (the less significant bit is 0, the most significant bit is 31).. Memory capacity is 2^25, cell addresses are 0 - (2^25 - 1). The size of each cell is 8 bits (byte). This machine works only with positive and negative integers. The arithmetics is done using the “Two’s complement” method. Characters represented in ascii are allowed.

Machine logic: Our machine has a variety of commands. Each command is assembled by Action and Operands. Each command is coded to a 32 bit string in Machine Code. Each command is being stored in the machine memory storage with a capacity of 4 straight bytes, using the “Little Endian” method. Each command has a command code, symbolized as “opcode”, that recognizes the command required action (some of the commands have a sub code called “funct”). The commands are divided into 3 command types: R, I and J.

Structure of machine code: Each of our assembly machine code commands contains the command data that is being stored in 32 bits. The size of all of the commands are the same, while the order of the data in its representation varies among the different command types.

Translation process: Our assembler holds a table that contains all of the commands and actions described before. These commands and actions are being translated into binary representation, as well as data that is being stored in the registers. The Translation process contains 2 phases: In the First phase, the assembler goes over all of the Source Code, and builds a “Symbols Table” and inserts to it each symbol that was defined in the source code, with numeric data (its location in the machine memory). In the Second phase, the assembler goes over all of the Source Code, and uses the Symbols Table to translate the lines in the source code, to the desired “Machine Code”.

In order to keep our code readable as possible, we have used this conventions and shortcuts: IC - stands for Instruction Counter. DC - stands for Data Counter. In the assembler.h file we defined most of the numeric constants we are using in this project.

Running and execution: The first run is defined in the first_pass.c file. This file contains the algorithm we have used to go over the source code for the first time, and it executes the logic we have described earlier, that sets the machine to be ready for all of the setups for the second phase. The second run is defined in the second_pass.c file. This file contains the algorithm we have used to go over the source code for the second time, and it executes the logic we have described earlier, that eventually creates the machine code.

//// -----> will finish this part shortly Input files and activating the assembler:

Output files: