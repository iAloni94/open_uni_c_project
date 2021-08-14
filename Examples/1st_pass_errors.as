ERR: .asciz "Error_Test
.asciz Error_Test"
.asciz "Error_T"est
.dw 2147483648
.dw -2147483649
.dh 32768
.db -129
mult $2, $3, $4
add $11, ; $8, $19
subi $15, 55, 
bne $29, $0
.dh 
.dh asdf
sub $24, $11, $30, $2
nor $8, $1, $12, 
Labe!: add $27, $13, $30
;EXT: .extern end
ERR: nori $7, 2, $4
.extern ERR
add: la ERR