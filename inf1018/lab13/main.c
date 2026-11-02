#include <stdio.h>
#include "execpage.h"

typedef int (*funcp)(int x);

int add(int x) { return x + 1; }

int main(void) {
    unsigned char cod1[] = {0x89, 0xf8, 0x83, 0xc0, 0x01, 0xc3};
    funcp f1 = (funcp)cod1;
    printf("Parte 2  (x+1)     : (*f)(10) = %d\n", (*f1)(10));
    static unsigned char cod2[] = {0xe8, 0x00, 0x00, 0x00, 0x00, 0xc3};
    if (execpage(cod2, sizeof cod2) != 0) { perror("execpage"); return 1; }
    long d2 = (unsigned char*)add - (cod2 + 5);
    cod2[1] = d2 & 0xff; cod2[2] = (d2>>8) & 0xff;
    cod2[3] = (d2>>16) & 0xff; cod2[4] = (d2>>24) & 0xff;
    funcp f2 = (funcp)cod2;
    printf("Parte 4  (call add): (*f)(10) = %d\n", (*f2)(10));
    static unsigned char cod3[] = {0xe9, 0x00, 0x00, 0x00, 0x00};
    if (execpage(cod3, sizeof cod3) != 0) { perror("execpage"); return 1; }
    long d3 = (unsigned char*)add - (cod3 + 5);
    cod3[1] = d3 & 0xff; cod3[2] = (d3>>8) & 0xff;
    cod3[3] = (d3>>16) & 0xff; cod3[4] = (d3>>24) & 0xff;
    funcp f3 = (funcp)cod3;
    printf("Parte 5  (jmp add) : (*f)(10) = %d\n", (*f3)(10));

    return 0;
}