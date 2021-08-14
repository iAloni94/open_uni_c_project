.extern IDO
jmp IDO
sw $3, 24, $7
.asciz "C_IS_SO_COOL_WOW"
.db 69
.dw 123456
la ASSEMBEL
MAIN: add $1, $2, $3
; This is a comment...
L2: jmp $23
.extern ASSEMBEL
L3: beq $5, $18, MAIN
andi $30, 126, $21
L: call IDO
.entry L
FIB: .dh 1,1,2,3,5,8,13,21,34,55,89,144
END: stop
.entry L2
.entry L3
sb $29, -20, $23
bgt $18, $20, L2
la ASSEMBEL
call FIB
mvhi $6, $18
or $1, $2, $3