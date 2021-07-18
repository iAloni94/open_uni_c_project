/* this file is for getting ready for the main function to do its thing */

#include "input.h"
#include "param.h"

void add_func(node_t *input){
    int reg1 = *(input->val);
    int reg2 = *(input->next->val);
    int reg3 = *(input->next->next->val);;

    /*regArray[reg3] = add(regArray[reg1], regArray[reg2]); */

}
void sub_func(node_t *input){}
void and_func(node_t *input){}
void or_func(node_t *input){}
void nor_func(node_t *input){}
void move_func(node_t *input){}
void mvhi_func(node_t *input){}
void mvlo_func(node_t *input){}

void undef_func(){}