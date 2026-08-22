#include <stdio.h>

unsigned char rotate_left(unsigned char x, int n) {
    return (x << n) | (x >> (8 - n));
}

int main() {
    unsigned char valor = 0x61; //0110 0001
    //teste 1: Rotate de 1 bit (esperado: 0xc2)
    printf("Rotate left 1 bit:  0x%x\n", rotate_left(valor, 1));
    //teste 2: Rotate de 2 bits (esperado: 0x85)
    printf("Rotate left 2 bits: 0x%x\n", rotate_left(valor, 2));
    //teste 3: Rotate de 7 bits (esperado: 0xb0)
    printf("Rotate left 7 bits: 0x%x\n", rotate_left(valor, 7));
    return 0;
}