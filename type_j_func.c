#include "global.h"

void jmp_func(J *instruction) {
    return;
}
void la_func(J *instruction) {
    regArray[0]->val = instruction->address;
}
void call_func(J *instruction) {
    regArray[0]->val = instruction->address;
}
void stop_func(J *instruction) { 
    return;
}  