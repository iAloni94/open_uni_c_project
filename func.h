#define CHECK_BIT(reg, n) ((reg)&1 << n) ? true : false         /* checks if bit in nth place from right is true or false */
#define CHANGE_BIT(reg, v, n) (reg) ^= (-v ^ reg) & (1UL << n); /* chnage nth bit in reg to v (0 or 1) */

void add_func();
void sub_func();
void and_func(); 
void or_func();
void nor_func();
void move_func();
void mvhi_func(); 
void mvlo_func();
void addi_func();
void subi_func();
void andi_func();
void ori_func();
void nori_func();
void bne_func();
void beq_func();
void blt_func();
void bgt_func();
void lb_func();
void sb_func();
void lw_func();
void sw_func();
void lh_func();
void sh_func();
void jmp_func();
void la_func();
void call_func();
void stop_func();
void undef_func();