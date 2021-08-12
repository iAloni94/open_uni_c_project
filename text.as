MAIN: add	$3,$5,$9
LOOP: ori	$9,-5,$2
la	val1
jmp	Next
Next:	move $20,$4
LIST:   .db	6,-9
la	K
sw	$0,4,$10
bne	$31,$9, LOOP