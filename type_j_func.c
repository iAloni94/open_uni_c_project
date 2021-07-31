#include "assembler.h"
#include "func.h"
#include "param.h"
#include "utils.h"

void jmp_func(J *instruction) {
    return;
}
void la_func(J *instruction) {
    regArray[0]->val = instruction->address;
}
void call_func(J *instruction, unsigned int IC) {
    regArray[0]->val = IC + 4;
}
void stop_func(J *instruction) {
    return;
}