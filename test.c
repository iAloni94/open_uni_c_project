#include <stdio.h>

unsigned int twosComp(unsigned int val){
    unsigned int res = (~val)+1;

    return res;
}


int main(){
    unsigned int a = -5;
    unsigned int res = twosComp(a);
    res = twosComp(res);
    printf("%d", res);
return 1;
}